#include "mpu6050.h"


int8_t mpu6050_setup(void)
{
    char buf[2];

    /* setup */
    i2c_setup();
    bcm2835_i2c_setSlaveAddress(MPU6050_ADDRESS);

    /* POWER MANAGEMENT */
    buf[0] = MPU6050_RA_PWR_MGMT_1;
    buf[1] = 0x00;
    bcm2835_i2c_write(buf, 2);

    return 0;
}

void mpu6050_teardown(void)
{
    i2c_teardown();
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

int8_t mpu6050_data(struct mpu6050_data *data)
{
    char buf[1];
    char raw_data[14];
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
    data->accel.x = (raw_data[0] << 8) | (raw_data[1]);
    data->accel.y = (raw_data[2] << 8) | (raw_data[3]);
    data->accel.z = (raw_data[4] << 8) | (raw_data[5]);

    /* calculate roll */
    /* roll = arctan(accel_x / sqrt(pow(accel_y, 2) + pow(accel_z, 2))) */

    /* calculate pitch */
    /* pitch = arctan(accel_y / sqrt(pow(accel_x, 2) + pow(accel_z, 2))) */

    /* temperature */
    data->temperature = (raw_data[6] << 8) | (raw_data[7]);
    data->temperature = (data->temperature / 340) + 36.53;

    /* gyroscope */
    data->gyro.x = (raw_data[8] << 8) | (raw_data[9]);
    data->gyro.y = (raw_data[10] << 8) | (raw_data[11]);
    data->gyro.z = (raw_data[12] << 8) | (raw_data[13]);

    return 0;
}

int8_t mpu6050_calibrate(void)
{
    int16_t i;
    int16_t offset_x;
    int16_t offset_y;
    int16_t offset_z;
    struct mpu6050_data data;

    for (i = 0; i < 5000; i++) {
        mpu6050_data(&data);

        offset_x += data.gyro.x;
        offset_y += data.gyro.y;
        offset_z += data.gyro.z;
    }

    offset_x = offset_x / 5000;
    offset_y = offset_y / 5000;
    offset_z = offset_z / 5000;

    printf("calibrate mpu6050\n");
    printf("offset_x: %d\n", offset_x);
    printf("offset_y: %d\n", offset_y);
    printf("offset_z: %d\n", offset_z);

    return 0;
}

void mpu6050_data_print(struct mpu6050_data *data)
{
    printf("gyro_x: %d\n", data->gyro.x);
    printf("gyro_y: %d\n", data->gyro.y);
    printf("gyro_z: %d\n", data->gyro.z);

    printf("accel x: %d\n", data->accel.x);
    printf("accel y: %d\n", data->accel.y);
    printf("accel z: %d\n", data->accel.z);

    printf("temp: %f\n", data->temperature);
    printf("\n");
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

    /* set sample rate */
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
