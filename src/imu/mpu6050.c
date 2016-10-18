#include "piq/imu/mpu6050.h"


int8_t mpu6050_setup(struct mpu6050 *sensor, struct i2c *conn)
{
    int8_t retval;

    /* setup */
    sensor->conn = conn;
    i2c_set_slave(sensor->conn, MPU6050_ADDRESS);

    /* set intial values */
    sensor->gyro.offset_x = 0.0f;
    sensor->gyro.offset_y = 0.0f;
    sensor->gyro.offset_z = 0.0f;
    sensor->gyro.pitch = 0.0f;
    sensor->gyro.roll = 0.0f;

    sensor->accel.offset_x = 0.0f;
    sensor->accel.offset_y = 0.0f;
    sensor->accel.offset_z = 0.0f;
    sensor->accel.pitch = 0.0f;
    sensor->accel.roll = 0.0f;

    sensor->pitch_offset = 0.0f;
    sensor->roll_offset = 0.0f;

    sensor->pitch = 0.0f;
    sensor->roll = 0.0f;
    sensor->temperature = 0.0f;

    sensor->last_updated = clock();
    sensor->sample_rate = -1.0;
    sensor->dplf_config = 0;

    /* set dplf */
    mpu6050_set_dplf_config(sensor, 6);
    retval = mpu6050_get_dplf_config(sensor);
    if (retval > 7 || retval < 0) {
        return -1;
    } else {
        sensor->dplf_config = retval;
        log_info("dplf config: %d", sensor->dplf_config);
    }

    /* set power management register */
    i2c_write_byte(sensor->conn, MPU6050_RA_PWR_MGMT_1, 0x00);

    /* get gyro range */
    mpu6050_set_gyro_range(sensor, 0);
    retval = mpu6050_get_gyro_range(sensor);
    if (retval == 0) {
        sensor->gyro.sensitivity = 131.0;
    } else if (retval == 1) {
        sensor->gyro.sensitivity = 65.5;
    } else if (retval == 2) {
        sensor->gyro.sensitivity = 32.8;
    } else if (retval == 3) {
        sensor->gyro.sensitivity = 16.4;
    } else {
        return -2;
    }

    /* get accel range */
    mpu6050_set_accel_range(sensor, 0);
    retval = mpu6050_get_accel_range(sensor);
    if (retval == 0) {
        sensor->accel.sensitivity = 16384.0;
    } else if (retval == 1) {
        sensor->accel.sensitivity = 8192.0;
    } else if (retval == 2) {
        sensor->accel.sensitivity = 4096.0;
    } else if (retval == 3) {
        sensor->accel.sensitivity = 2048.0;
    } else {
        return -3;
    }

    /* get sample rate */
    sensor->sample_rate = mpu6050_get_sample_rate(sensor);

    /* calibrate mpu6050 */
    mpu6050_calibrate(sensor);

    return 0;
}

int8_t mpu6050_ping(struct mpu6050 *sensor)
{
    char buf[1];

    /* print mpu6050 address */
    buf[0] = 0x00;
    i2c_set_slave(sensor->conn, MPU6050_ADDRESS);
    i2c_read_bytes(sensor->conn, MPU6050_RA_WHO_AM_I, buf, 1);
    printf("MPU6050 ADDRESS: 0x%02X\n", buf[0]);

    return 0;
}

static void accelerometer_calc_angle(struct mpu6050 *sensor)
{
    float x;
    float y;
    float z;

    /* setup */
    x = sensor->accel.x;
    y = sensor->accel.y;
    z = sensor->accel.z;

    /* calculate pitch and roll */
    sensor->accel.pitch = (atan(x / sqrt(pow(y, 2) + pow(z, 2)))) * 180 / M_PI;
    sensor->accel.roll = (atan(y / sqrt(pow(x, 2) + pow(z, 2)))) * 180 / M_PI;
}

static void gyroscope_calc_angle(struct mpu6050 *sensor, float dt)
{
    sensor->gyro.roll = (sensor->gyro.x * dt) + sensor->roll;
    sensor->gyro.pitch = (sensor->gyro.y * dt) + sensor->pitch;
}

int8_t mpu6050_data(struct mpu6050 *sensor)
{
    char raw_data[14];
    int8_t raw_temp;
    float dt;
    clock_t time_now;
    int retval;

    /* read sensor data */
    memset(raw_data, '\0', 14);
    i2c_set_slave(sensor->conn, MPU6050_ADDRESS);
    retval = i2c_read_bytes(sensor->conn, MPU6050_RA_ACCEL_XOUT_H, raw_data, 14);
    if (retval != 0) {
        return -1;
    }

    /* accelerometer */
    sensor->accel.raw_x = (raw_data[0] << 8) | (raw_data[1]);
    sensor->accel.raw_y = (raw_data[2] << 8) | (raw_data[3]);
    sensor->accel.raw_z = (raw_data[4] << 8) | (raw_data[5]);

    sensor->accel.raw_x -= sensor->accel.offset_x;
    sensor->accel.raw_y -= sensor->accel.offset_y;
    sensor->accel.raw_z -= sensor->accel.offset_z;

    sensor->accel.x = sensor->accel.raw_x / sensor->accel.sensitivity;
    sensor->accel.y = sensor->accel.raw_y / sensor->accel.sensitivity;
    sensor->accel.z = sensor->accel.raw_z / sensor->accel.sensitivity;

    /* temperature */
    raw_temp = (raw_data[6] << 8) | (raw_data[7]);
    sensor->temperature = raw_temp / 340.0 + 36.53;

    /* gyroscope */
    sensor->gyro.raw_x = (raw_data[8] << 8) | (raw_data[9]);
    sensor->gyro.raw_y = (raw_data[10] << 8) | (raw_data[11]);
    sensor->gyro.raw_z = (raw_data[12] << 8) | (raw_data[13]);

    sensor->gyro.raw_x -= sensor->gyro.offset_x;
    sensor->gyro.raw_y -= sensor->gyro.offset_y;
    sensor->gyro.raw_z -= sensor->gyro.offset_z;

    sensor->gyro.x = sensor->gyro.raw_x / sensor->gyro.sensitivity;
    sensor->gyro.y = sensor->gyro.raw_y / sensor->gyro.sensitivity;
    sensor->gyro.z = sensor->gyro.raw_z / sensor->gyro.sensitivity;


    /* calculate dt */
    time_now = clock();
    dt = ((double) time_now - sensor->last_updated) / CLOCKS_PER_SEC;

    /* complimentary filter */
    accelerometer_calc_angle(sensor);
    sensor->pitch = (0.98 * sensor->gyro.pitch) + (0.02 * sensor->accel.pitch);
    sensor->roll = (0.98 * sensor->gyro.roll) + (0.02 * sensor->accel.roll);
    gyroscope_calc_angle(sensor, dt);

    /* offset pitch and roll */
    sensor->pitch += sensor->pitch_offset;
    sensor->roll += sensor->roll_offset;

    /* set last_updated */
    sensor->last_updated = clock();

    return 0;
}

int8_t mpu6050_calibrate(struct mpu6050 *sensor)
{
    int16_t i;

    /* let it stablize for a while first */
    log_info("calibrating mpu6050");
    for (i = 0; i < 50; i++) {
        mpu6050_data(sensor);
    }

    /* calculate offset */
    for (i = 0; i < 50; i++) {
        mpu6050_data(sensor);

        sensor->accel.offset_x += sensor->accel.raw_x;
        sensor->accel.offset_y += sensor->accel.raw_y;
        sensor->accel.offset_z += sensor->accel.raw_z;

        sensor->accel.offset_x = sensor->accel.offset_x / 2.0;
        sensor->accel.offset_y = sensor->accel.offset_y / 2.0;
        sensor->accel.offset_z = sensor->accel.offset_z / 2.0;

        sensor->gyro.offset_x += sensor->gyro.raw_x;
        sensor->gyro.offset_y += sensor->gyro.raw_y;
        sensor->gyro.offset_z += sensor->gyro.raw_z;

        sensor->gyro.offset_x = sensor->gyro.offset_x / 2.0;
        sensor->gyro.offset_y = sensor->gyro.offset_y / 2.0;
        sensor->gyro.offset_z = sensor->gyro.offset_z / 2.0;
    }

    return 0;
}

void mpu6050_print(struct mpu6050 *sensor)
{
    printf("gyro_x: %f\n", sensor->gyro.x);
    printf("gyro_y: %f\n", sensor->gyro.y);
    printf("gyro_z: %f\n", sensor->gyro.z);

    printf("accel x: %f\n", sensor->accel.x);
    printf("accel y: %f\n", sensor->accel.y);
    printf("accel z: %f\n", sensor->accel.z);

    printf("\n");
    printf("accel pitch: %f\n", sensor->accel.pitch);
    printf("accel roll: %f\n", sensor->accel.roll);
    printf("\n");
    printf("gyro pitch: %f\n", sensor->gyro.pitch);
    printf("gyro roll: %f\n", sensor->gyro.roll);
    printf("\n");

    printf("temp: %f\n", sensor->temperature);
    printf("\n");
    printf("\n");
}

int8_t mpu6050_set_dplf_config(struct mpu6050 *sensor, int8_t setting)
{
    int retval;

    /*
        DPLF_CFG    Accelerometer
        ----------------------------------------
                    Bandwidth(Hz) | Delay(ms)
        0           260             0
        1           184             2.0
        2           94              3.0
        3           44              4.9
        4           21              8.5
        5           10              13.8
        6           5               19.0
        7           RESERVED        RESERVED


        DPLF_CFG    Gyroscope
        ----------------------------------------------
                    Bandwidth(Hz) | Delay(ms) | Fs(kHz)
        0           256             0.98        8
        1           188             1.9         1
        2           98              2.8         1
        3           42              4.8         1
        4           20              8.3         1
        5           10              13.4        1
        6           5               18.5        1
        7           RESERVED        RESERVED    8
    */

    /* check setting range */
    if (setting > 7 || setting < 0) {
        return -2;
    }

    /* set DPLF */
    i2c_set_slave(sensor->conn, MPU6050_ADDRESS);
    retval = i2c_write_byte(sensor->conn, MPU6050_RA_CONFIG, (char) setting);
    if (retval != 0) {
        return -1;
    }

    return 0;
}

int8_t mpu6050_get_dplf_config(struct mpu6050 *sensor)
{
    char data[1];
    int retval;

    /* get dplf config */
    data[0] = 0x00;
    i2c_set_slave(sensor->conn, MPU6050_ADDRESS);
    retval = i2c_read_bytes(sensor->conn, MPU6050_RA_CONFIG, data, 1);
    if (retval != 0) {
        return -1;
    }

    log_info("GOT DPLF: %d", data[0]);
    data[0] = data[0] & 0b00000111;

    return data[0];
}

int8_t mpu6050_set_sample_rate_div(struct mpu6050 *sensor, int8_t div)
{
    char data[1];
    int retval;

    /* set sample rate divider */
    data[0] = div;
    i2c_set_slave(sensor->conn, MPU6050_ADDRESS);
    retval = i2c_write_bytes(sensor->conn, MPU6050_RA_SMPLRT_DIV, data, 1);
    if (retval != 0) {
        return -1;
    }

    return 0;
}

int8_t mpu6050_get_sample_rate_div(struct mpu6050 *sensor)
{
    char data[1];
    int retval;

    /* get sample rate */
    data[0] = 0x00;
    i2c_set_slave(sensor->conn, MPU6050_ADDRESS);
    retval = i2c_read_bytes(sensor->conn, MPU6050_RA_SMPLRT_DIV, data, 1);
    if (retval != 0) {
        return -1;
    }

    return data[0];
}

int16_t mpu6050_get_sample_rate(struct mpu6050 *sensor)
{
    uint8_t smplrt_div;
    uint8_t dlpf_cfg;
    uint16_t sample_divider;
    uint16_t gyro_rate;

    /* get sample rate divider */
    smplrt_div = mpu6050_get_sample_rate_div(sensor);
    if (smplrt_div != -1 || smplrt_div != -2) {
        sample_divider = (float) smplrt_div;
    } else {
        return -1;
    }

    /* get gyro sample rate */
    dlpf_cfg = mpu6050_get_sample_rate_div(sensor);
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

int8_t mpu6050_set_gyro_range(struct mpu6050 *sensor, int8_t range)
{
    char data;
    uint8_t retval;

    /* pre-check */
    if (range > 3 || range < 0) {
        return -2;
    }

    /* set sample rate */
    data = range << 3;
    i2c_set_slave(sensor->conn, MPU6050_ADDRESS);
    retval = i2c_write_byte(sensor->conn, MPU6050_RA_GYRO_CONFIG, data);
    if (retval != 0) {
        return -1;
    }

    return 0;
}

int8_t mpu6050_get_gyro_range(struct mpu6050 *sensor)
{
    char data;
    int retval;

    /* get gyro config */
    data = 0x00;
    i2c_set_slave(sensor->conn, MPU6050_ADDRESS);
    retval = i2c_read_byte(sensor->conn, MPU6050_RA_GYRO_CONFIG, &data);
    if (retval != 0) {
        return -1;
    }

    /* get gyro range bytes */
    data = (data >> 3) & 0b00000011;

    return data;
}

int8_t mpu6050_set_accel_range(struct mpu6050 *sensor, int8_t range)
{
    char data;
    uint8_t retval;

    /* pre-check */
    if (range > 3 || range < 0) {
        return -2;
    }

    /* set sample rate */
    data = range << 3;
    i2c_set_slave(sensor->conn, MPU6050_ADDRESS);
    retval = i2c_write_byte(sensor->conn, MPU6050_RA_ACCEL_CONFIG, data);
    if (retval != 0) {
        return -1;
    }

    return 0;
}

int8_t mpu6050_get_accel_range(struct mpu6050 *sensor)
{
    char data;
    uint8_t retval;

    /* get accel config */
    data = 0x00;
    i2c_set_slave(sensor->conn, MPU6050_ADDRESS);
    retval = i2c_read_byte(sensor->conn, MPU6050_RA_ACCEL_CONFIG, &data);
    if (retval != 0) {
        return -1;
    }

    /* get accel range bytes */
    data = (data >> 3) & 0b00000011;

    return data;
}

void mpu6050_info(struct mpu6050 *data)
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

static void mpu6050_record_header(struct mpu6050 *data, FILE *output_file)
{
    fprintf(output_file, "gyro.x,");
    fprintf(output_file, "gyro.y,");
    fprintf(output_file, "gyro.z,");
    fprintf(output_file, "gyro.pitch,");
    fprintf(output_file, "gyro.roll,");

    fprintf(output_file, "accel.x,");
    fprintf(output_file, "accel.y,");
    fprintf(output_file, "accel.z,");
    fprintf(output_file, "accel.pitch,");
    fprintf(output_file, "accel.roll,");

    fprintf(output_file, "pitch,");
    fprintf(output_file, "roll\n");
}

static void mpu6050_record_data(struct mpu6050 *data, FILE *output_file)
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
}

int8_t mpu6050_record(struct mpu6050 *sensor, char *output_path, int nb_samples)
{
    int i;
    int8_t retval;
    FILE *output_file;

    /* setup */
    output_file = fopen(output_path, "w");
    mpu6050_record_header(sensor, output_file);

    /* record */
    for (i = 0; i < nb_samples; i++) {
        /* get data */
        retval = mpu6050_data(sensor);
        if (retval == -1) {
            log_err("failed to obtain data from MPU6050!");
            return -1;
        }

        /* record data */
        mpu6050_record_data(sensor, output_file);
        if (retval == -1) {
            log_err("failed to record MPU6050 data!");
            return -1;
        }
    }

    /* clean up */
    fclose(output_file);

    return 0;
}
