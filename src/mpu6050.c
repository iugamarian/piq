#include "mpu6050.h"


int8_t mpu6050_setup(struct mpu6050_data *data)
{
    int8_t retval;
    char buf[2];

    /* setup */
    log_info("initializing mpu6050");
    bcm2835_i2c_setSlaveAddress(MPU6050_ADDRESS);

    /* set intial values */
    data->gyro.offset_x = 0;
    data->gyro.offset_y = 0;
    data->gyro.offset_z = 0;
    data->gyro.pitch = FLT_MIN;
    data->gyro.roll = FLT_MIN;

    data->accel.offset_x = 0;
    data->accel.offset_y = 0;
    data->accel.offset_z = 0;
    data->accel.pitch = FLT_MIN;
    data->accel.roll = FLT_MIN;

    data->pitch = 0;
    data->roll = 0;

    data->sample_rate = -1;

    /* set config register */
    buf[0] = MPU6050_RA_CONFIG;
    buf[1] = 0x00;
    bcm2835_i2c_write(buf, 2);

    /* set power management register */
    buf[0] = MPU6050_RA_PWR_MGMT_1;
    buf[1] = 0x00;
    bcm2835_i2c_write(buf, 2);

    /* get gyro range */
    mpu6050_set_gyro_range(0);
    retval = mpu6050_get_gyro_range();
    if (retval == 0) {
        data->gyro.sensitivity = 131.0;
    } else if (retval == 1) {
        data->gyro.sensitivity = 65.5;
    } else if (retval == 2) {
        data->gyro.sensitivity = 32.8;
    } else if (retval == 3) {
        data->gyro.sensitivity = 16.4;
    }

    /* get accel range */
    mpu6050_set_accel_range(0);
    retval = mpu6050_get_accel_range();
    if (retval == 0) {
        data->accel.sensitivity = 16384.0;
    } else if (retval == 1) {
        data->accel.sensitivity = 8192.0;
    } else if (retval == 2) {
        data->accel.sensitivity = 4096.0;
    } else if (retval == 3) {
        data->accel.sensitivity = 2048.0;
    }

    /* get sample rate */
    data->sample_rate = mpu6050_get_sample_rate();

    /* calibrate mpu6050 */
    mpu6050_calibrate(data);

    return 0;
}

int8_t mpu6050_ping(void)
{
    char buf[1];
    char value[1];

    /* setup */
    buf[0] = MPU6050_RA_WHO_AM_I;
    value[0] = 0x00;

    /* print mpu6050 address */
    bcm2835_i2c_read_register_rs(buf, value, 1);
    printf("MPU6050 ADDRESS: 0x%02X\n", value[0]);

    return 0;
}

static void accelerometer_calc_angle(struct mpu6050_data *data)
{
    float x;
    float y;
    float z;

    /* setup */
    x = data->accel.x;
    y = data->accel.y;
    z = data->accel.z;

    /* calculate pitch and roll */
    data->accel.pitch = (atan(x / sqrt(pow(y, 2) + pow(z, 2)))) * 180 / M_PI ;
    data->accel.roll = (atan(y / sqrt(pow(x, 2) + pow(z, 2)))) * 180 / M_PI;
}

static void gyroscope_calc_angle(struct mpu6050_data *data, float dt)
{
    data->gyro.roll = (data->gyro.x * dt) + data->roll;
    data->gyro.pitch = (data->gyro.y * dt) + data->pitch;
}

int8_t mpu6050_data(struct mpu6050_data *data)
{
    char buf[1];
    char raw_data[14];
    int8_t raw_temp;
    float dt;
    clock_t time_now;
    bcm2835I2CReasonCodes retval;

    /* setup */
    buf[0] = MPU6050_RA_ACCEL_XOUT_H;
    memset(raw_data, 0, 14);

    /* read sensor data */
    retval = bcm2835_i2c_write_read_rs(buf, 1, raw_data, 14);
    if (retval != BCM2835_I2C_REASON_OK) {
        return -1;
    }

    /* accelerometer */
    data->accel.raw_x = (raw_data[0] << 8) | (raw_data[1]);
    data->accel.raw_y = (raw_data[2] << 8) | (raw_data[3]);
    data->accel.raw_z = (raw_data[4] << 8) | (raw_data[5]);

    data->accel.raw_x -= data->accel.offset_x;
    data->accel.raw_y -= data->accel.offset_y;
    data->accel.raw_z -= data->accel.offset_z;

    data->accel.x = data->accel.raw_x / data->accel.sensitivity;
    data->accel.y = data->accel.raw_y / data->accel.sensitivity;
    data->accel.z = data->accel.raw_z / data->accel.sensitivity;

    /* temperature */
    raw_temp = (raw_data[6] << 8) | (raw_data[7]);
    data->temperature = raw_temp / 340.0 + 36.53;

    /* gyroscope */
    data->gyro.raw_x = (raw_data[8] << 8) | (raw_data[9]);
    data->gyro.raw_y = (raw_data[10] << 8) | (raw_data[11]);
    data->gyro.raw_z = (raw_data[12] << 8) | (raw_data[13]);

    data->gyro.raw_x -= data->gyro.offset_x;
    data->gyro.raw_y -= data->gyro.offset_y;
    data->gyro.raw_z -= data->gyro.offset_z;

    data->gyro.x = data->gyro.raw_x / data->gyro.sensitivity;
    data->gyro.y = data->gyro.raw_y / data->gyro.sensitivity;
    data->gyro.z = data->gyro.raw_z / data->gyro.sensitivity;

    /* calculate dt */
    time_now = clock();
    dt = ((double) time_now - data->last_updated) / CLOCKS_PER_SEC;

    /* complimentary filter */
    accelerometer_calc_angle(data);
    data->pitch = (0.98 * data->gyro.pitch) + (0.02 * data->accel.pitch);
    data->roll = (0.98 * data->gyro.roll) + (0.02 * data->accel.roll);
    gyroscope_calc_angle(data, dt);

    /* set last_updated */
    data->last_updated = clock();

    return 0;
}

int8_t mpu6050_calibrate(struct mpu6050_data *data)
{
    int16_t i;

    /* let it stablize for a while first */
    log_info("calibrating mpu6050");
    for (i = 0; i < 5000; i++) {
        mpu6050_data(data);
    }

    /* calculate offset */
    for (i = 0; i < 5000; i++) {
        mpu6050_data(data);

        data->accel.offset_x += data->accel.raw_x;
        data->accel.offset_y += data->accel.raw_y;
        data->accel.offset_z += data->accel.raw_z;

        data->gyro.offset_x += data->gyro.raw_x;
        data->gyro.offset_y += data->gyro.raw_y;
        data->gyro.offset_z += data->gyro.raw_z;
    }

    data->accel.offset_x = data->accel.offset_x / 5000.0;
    data->accel.offset_y = data->accel.offset_y / 5000.0;
    data->accel.offset_z = data->accel.offset_z / 5000.0;

    data->gyro.offset_x = data->gyro.offset_x / 5000.0;
    data->gyro.offset_y = data->gyro.offset_y / 5000.0;
    data->gyro.offset_z = data->gyro.offset_z / 5000.0;

    return 0;
}

void mpu6050_data_print(struct mpu6050_data *data)
{
    printf("gyro_x: %f\n", data->gyro.x);
    printf("gyro_y: %f\n", data->gyro.y);
    printf("gyro_z: %f\n", data->gyro.z);

    printf("accel x: %f\n", data->accel.x);
    printf("accel y: %f\n", data->accel.y);
    printf("accel z: %f\n", data->accel.z);

    printf("\n");
    printf("accel pitch: %f\n", data->accel.pitch);
    printf("accel roll: %f\n", data->accel.roll);
    printf("\n");
    printf("gyro pitch: %f\n", data->gyro.pitch);
    printf("gyro roll: %f\n", data->gyro.roll);
    printf("\n");

    printf("temp: %f\n", data->temperature);
    printf("\n");
    printf("\n");
}


int16_t mpu6050_get_sample_rate(void)
{
    uint8_t smplrt_div;
    uint8_t dlpf_cfg;
    uint16_t sample_divider;
    uint16_t gyro_rate;

    /* get sample rate divider */
    smplrt_div = mpu6050_get_sample_rate_div();
    if (smplrt_div != -1 || smplrt_div != -2) {
        sample_divider = (float) smplrt_div;
    } else {
        return -1;
    }

    /* get gyro sample rate */
    dlpf_cfg = mpu6050_get_sample_rate_div();
    if (dlpf_cfg == 0 || dlpf_cfg == 7) {
        gyro_rate = 8000;
    } else if (dlpf_cfg >= 1 || dlpf_cfg <= 6) {
        gyro_rate = 1000;
    } else {
        return -2;
    }

    /* calculate sample rate */
    return gyro_rate / (1 + sample_divider);
}

int8_t mpu6050_get_sample_rate_div(void)
{
    char buf[1];
    char data[1];
    uint8_t retval;

    /* get sample rate */
    buf[0] = MPU6050_RA_SMPLRT_DIV;
    retval = bcm2835_i2c_write_read_rs(buf, 1, data, 1);
    if (retval != BCM2835_I2C_REASON_OK) {
        return -1;
    }

    return data[0];
}

int8_t mpu6050_set_sample_rate_div(int8_t div)
{
    char buf[1];
    uint8_t retval;

    /* set sample rate divider */
    buf[0] = MPU6050_RA_SMPLRT_DIV;
    buf[1] = div;
    retval = bcm2835_i2c_write(buf, 2);
    if (retval != BCM2835_I2C_REASON_OK) {
        return -1;
    }

    return 0;
}

int8_t mpu6050_get_gyro_range(void)
{
    char buf[1];
    char data[1];
    uint8_t retval;

    /* get gyro config */
    buf[0] = MPU6050_RA_GYRO_CONFIG;
    retval = bcm2835_i2c_write_read_rs(buf, 1, data, 1);
    if (retval != BCM2835_I2C_REASON_OK) {
        return -1;
    }

    /* get gyro range bits */
    data[0] = (data[0] >> 3) & 0b00000011;

    return data[0];
}

int8_t mpu6050_set_gyro_range(int8_t range)
{
    char buf[1];
    uint8_t retval;

    /* pre-check */
    if (range > 3 || range < 0) {
        return -2;
    }

    /* set sample rate */
    buf[0] = MPU6050_RA_GYRO_CONFIG;
    buf[1] = range << 3;
    retval = bcm2835_i2c_write(buf, 2);
    if (retval != BCM2835_I2C_REASON_OK) {
        return -1;
    }

    return 0;
}

int8_t mpu6050_get_accel_range(void)
{
    char buf[1];
    char data[1];
    uint8_t retval;

    /* get accel config */
    buf[0] = MPU6050_RA_ACCEL_CONFIG;
    retval = bcm2835_i2c_write_read_rs(buf, 1, data, 1);
    if (retval != BCM2835_I2C_REASON_OK) {
        return -1;
    }

    /* get accel range bits */
    data[0] = (data[0] >> 3) & 0b00000011;

    return data[0];
}

int8_t mpu6050_set_accel_range(int8_t range)
{
    char buf[1];
    uint8_t retval;

    /* pre-check */
    if (range > 3 || range < 0) {
        return -2;
    }

    /* set sample rate */
    buf[0] = MPU6050_RA_ACCEL_CONFIG;
    buf[1] = range << 3;
    retval = bcm2835_i2c_write(buf, 2);
    if (retval != BCM2835_I2C_REASON_OK) {
        return -1;
    }

    return 0;
}

void mpu6050_info(struct mpu6050_data *data)
{
    printf("gyro sensitivity: %f\n", data->gyro.sensitivity);
    printf("gyro offset_x: %f\n", data->gyro.offset_x);
    printf("gyro offset_y: %f\n", data->gyro.offset_y);
    printf("gyro offset_z: %f\n", data->gyro.offset_z);
    printf("\n");
    printf("accel sensitivity: %f\n", data->accel.sensitivity);
    printf("accel offset_x: %f\n", data->accel.offset_x);
    printf("accel offset_y: %f\n", data->accel.offset_y);
    printf("accel offset_z: %f\n", data->accel.offset_z);
    printf("\n");
    printf("sample rate: %d\n", data->sample_rate);
    printf("\n");
}

int8_t mpu6050_record_data(FILE *output_file, struct mpu6050_data *data)
{
    fprintf(output_file, "%f,", data->gyro.x);
    fprintf(output_file, "%f,", data->gyro.y);
    fprintf(output_file, "%f,", data->gyro.z);
    fprintf(output_file, "%f,", data->gyro.pitch);
    fprintf(output_file, "%f,", data->gyro.roll);

    fprintf(output_file, "%f,", data->accel.x);
    fprintf(output_file, "%f,", data->accel.y);
    fprintf(output_file, "%f,", data->accel.z);
    fprintf(output_file, "%f,", data->accel.pitch);
    fprintf(output_file, "%f,", data->accel.roll);

    fprintf(output_file, "%f,", data->pitch);
    fprintf(output_file, "%f\n", data->roll);

    return 0;
}
