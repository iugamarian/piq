#include "mpu9250.h"


void mpu9250_setup(struct mpu9250 *imu)
{
    imu->gyro.offset_x = 0.0f;
    imu->gyro.offset_y = 0.0f;
    imu->gyro.offset_z = 0.0f;
    imu->gyro.pitch = 0.0f;
    imu->gyro.roll = 0.0f;

    imu->accel.offset_x = 0.0f;
    imu->accel.offset_y = 0.0f;
    imu->accel.offset_z = 0.0f;
    imu->accel.pitch = 0.0f;
    imu->accel.roll = 0.0f;

    imu->pitch = 0.0f;
    imu->roll = 0.0f;

    imu->sample_rate = -1.0;
}

int8_t mpu9250_ping(void)
{
    char data[1];

    /* print mpu9250 address */
    data[0] = 0x00;
    i2c_set_slave(MPU9250_ADDRESS);
    i2c_read_bytes(MPU9250_WHO_AM_I, data, 1);
    printf("MPU9250 ADDRESS: 0x%02X\n", data[0]);

    return data[0];
}

/* static void accelerometer_calc_angle(struct mpu9250_data *data) */
/* { */
/*     float x; */
/*     float y; */
/*     float z; */
/*  */
/*     #<{(| setup |)}># */
/*     x = data->accel->x; */
/*     y = data->accel->y; */
/*     z = data->accel->z; */
/*  */
/*     #<{(| calculate pitch and roll |)}># */
/*     data->accel->pitch = (atan(x / sqrt(pow(y, 2) + pow(z, 2)))) * 180 / M_PI; */
/*     data->accel->roll = (atan(y / sqrt(pow(x, 2) + pow(z, 2)))) * 180 / M_PI; */
/* } */
/*  */
/* static void gyroscope_calc_angle(struct mpu9250_data *data, float dt) */
/* { */
/*     data->gyro->roll = (data->gyro->x * dt) + data->roll; */
/*     data->gyro->pitch = (data->gyro->y * dt) + data->pitch; */
/* } */
/*  */
/* int8_t mpu9250_data(struct mpu9250_data *data) */
/* { */
/*     char raw_data[14]; */
/*     int8_t raw_temp; */
/*     float dt; */
/*     clock_t time_now; */
/*     int retval; */
/*  */
/*     #<{(| read sensor data |)}># */
/*     memset(raw_data, 0, 14); */
/*     i2c_set_slave(MPU9250_ADDRESS); */
/*     retval = i2c_read_bytes(MPU9250_RA_ACCEL_XOUT_H, raw_data, 14); */
/*     if (retval != 0) { */
/*         return -1; */
/*     } */
/*  */
/*     #<{(| accelerometer |)}># */
/*     data->accel->raw_x = (raw_data[0] << 8) | (raw_data[1]); */
/*     data->accel->raw_y = (raw_data[2] << 8) | (raw_data[3]); */
/*     data->accel->raw_z = (raw_data[4] << 8) | (raw_data[5]); */
/*  */
/*     data->accel->raw_x -= data->accel->offset_x; */
/*     data->accel->raw_y -= data->accel->offset_y; */
/*     data->accel->raw_z -= data->accel->offset_z; */
/*  */
/*     data->accel->x = data->accel->raw_x / data->accel->sensitivity; */
/*     data->accel->y = data->accel->raw_y / data->accel->sensitivity; */
/*     data->accel->z = data->accel->raw_z / data->accel->sensitivity; */
/*  */
/*     #<{(| temperature |)}># */
/*     raw_temp = (raw_data[6] << 8) | (raw_data[7]); */
/*     data->temperature = raw_temp / 340.0 + 36.53; */
/*  */
/*     #<{(| gyroscope |)}># */
/*     data->gyro->raw_x = (raw_data[8] << 8) | (raw_data[9]); */
/*     data->gyro->raw_y = (raw_data[10] << 8) | (raw_data[11]); */
/*     data->gyro->raw_z = (raw_data[12] << 8) | (raw_data[13]); */
/*  */
/*     data->gyro->raw_x -= data->gyro->offset_x; */
/*     data->gyro->raw_y -= data->gyro->offset_y; */
/*     data->gyro->raw_z -= data->gyro->offset_z; */
/*  */
/*     data->gyro->x = data->gyro->raw_x / data->gyro->sensitivity; */
/*     data->gyro->y = data->gyro->raw_y / data->gyro->sensitivity; */
/*     data->gyro->z = data->gyro->raw_z / data->gyro->sensitivity; */
/*  */
/*  */
/*     #<{(| calculate dt |)}># */
/*     time_now = clock(); */
/*     dt = ((double) time_now - data->last_updated) / CLOCKS_PER_SEC; */
/*  */
/*     #<{(| complimentary filter |)}># */
/*     accelerometer_calc_angle(data); */
/*     data->pitch = (0.98 * data->gyro->pitch) + (0.02 * data->accel->pitch); */
/*     data->roll = (0.98 * data->gyro->roll) + (0.02 * data->accel->roll); */
/*     gyroscope_calc_angle(data, dt); */
/*  */
/*     #<{(| offset pitch and roll |)}># */
/*     data->pitch += data->pitch_offset; */
/*     data->roll += data->roll_offset; */
/*  */
/*     #<{(| set last_updated |)}># */
/*     data->last_updated = clock(); */
/*  */
/*     return 0; */
/* } */
/*  */
/* int8_t mpu9250_calibrate(struct mpu9250_data *data) */
/* { */
/*     int16_t i; */
/*  */
/*     #<{(| let it stablize for a while first |)}># */
/*     log_info("calibrating mpu9250"); */
/*     for (i = 0; i < 5000; i++) { */
/*         mpu9250_data(data); */
/*     } */
/*  */
/*     #<{(| calculate offset |)}># */
/*     for (i = 0; i < 5000; i++) { */
/*         mpu9250_data(data); */
/*  */
/*         data->accel->offset_x += data->accel->raw_x; */
/*         data->accel->offset_y += data->accel->raw_y; */
/*         data->accel->offset_z += data->accel->raw_z; */
/*  */
/*         data->accel->offset_x = data->accel->offset_x / 2.0; */
/*         data->accel->offset_y = data->accel->offset_y / 2.0; */
/*         data->accel->offset_z = data->accel->offset_z / 2.0; */
/*  */
/*         data->gyro->offset_x += data->gyro->raw_x; */
/*         data->gyro->offset_y += data->gyro->raw_y; */
/*         data->gyro->offset_z += data->gyro->raw_z; */
/*  */
/*         data->gyro->offset_x = data->gyro->offset_x / 2.0; */
/*         data->gyro->offset_y = data->gyro->offset_y / 2.0; */
/*         data->gyro->offset_z = data->gyro->offset_z / 2.0; */
/*     } */
/*  */
/*     return 0; */
/* } */
/*  */
/* void mpu9250_data_print(struct mpu9250_data *data) */
/* { */
/*     printf("gyro_x: %f\n", data->gyro->x); */
/*     printf("gyro_y: %f\n", data->gyro->y); */
/*     printf("gyro_z: %f\n", data->gyro->z); */
/*  */
/*     printf("accel x: %f\n", data->accel->x); */
/*     printf("accel y: %f\n", data->accel->y); */
/*     printf("accel z: %f\n", data->accel->z); */
/*  */
/*     printf("\n"); */
/*     printf("accel pitch: %f\n", data->accel->pitch); */
/*     printf("accel roll: %f\n", data->accel->roll); */
/*     printf("\n"); */
/*     printf("gyro pitch: %f\n", data->gyro->pitch); */
/*     printf("gyro roll: %f\n", data->gyro->roll); */
/*     printf("\n"); */
/*  */
/*     printf("temp: %f\n", data->temperature); */
/*     printf("\n"); */
/*     printf("\n"); */
/* } */
/*  */
/* int8_t mpu9250_get_dplf_config(void) */
/* { */
/*     char data[1]; */
/*     int retval; */
/*  */
/*     #<{(| get dplf config |)}># */
/*     data[0] = 0x00; */
/*     i2c_set_slave(MPU9250_ADDRESS); */
/*     retval = i2c_read_bytes(MPU9250_RA_CONFIG, data, 1); */
/*     if (retval != 0) { */
/*         return -1; */
/*     } */
/*  */
/*     log_info("GET DPLF: %d", data[0]); */
/*     data[0] = data[0] & 0b00000111; */
/*  */
/*     return data[0]; */
/* } */
/*  */
/* int8_t mpu9250_set_dplf_config(int8_t setting) */
/* { */
/*     int retval; */
/*  */
/*     #<{(| */
/*         DPLF_CFG    Accelerometer */
/*         ---------------------------------------- */
/*                     Bandwidth(Hz) | Delay(ms) */
/*         0           260             0 */
/*         1           184             2.0 */
/*         2           94              3.0 */
/*         3           44              4.9 */
/*         4           21              8.5 */
/*         5           10              13.8 */
/*         6           5               19.0 */
/*         7           RESERVED        RESERVED */
/*  */
/*  */
/*         DPLF_CFG    Gyroscope */
/*         ---------------------------------------------- */
/*                     Bandwidth(Hz) | Delay(ms) | Fs(kHz) */
/*         0           256             0.98        8 */
/*         1           188             1.9         1 */
/*         2           98              2.8         1 */
/*         3           42              4.8         1 */
/*         4           20              8.3         1 */
/*         5           10              13.4        1 */
/*         6           5               18.5        1 */
/*         7           RESERVED        RESERVED    8 */
/*     |)}># */
/*  */
/*     #<{(| check setting range |)}># */
/*     if (setting > 7 || setting < 0) { */
/*         return -2; */
/*     } */
/*  */
/*     #<{(| set DPLF |)}># */
/*     i2c_set_slave(MPU9250_ADDRESS); */
/*     retval = i2c_write_byte(MPU9250_RA_CONFIG, (char) setting); */
/*     if (retval != 0) { */
/*         return -1; */
/*     } */
/*  */
/*     return 0; */
/* } */
/*  */
/* int16_t mpu9250_get_sample_rate(void) */
/* { */
/*     uint8_t smplrt_div; */
/*     uint8_t dlpf_cfg; */
/*     uint16_t sample_divider; */
/*     uint16_t gyro_rate; */
/*  */
/*     #<{(| get sample rate divider |)}># */
/*     smplrt_div = mpu9250_get_sample_rate_div(); */
/*     if (smplrt_div != -1 || smplrt_div != -2) { */
/*         sample_divider = (float) smplrt_div; */
/*     } else { */
/*         return -1; */
/*     } */
/*  */
/*     #<{(| get gyro sample rate |)}># */
/*     dlpf_cfg = mpu9250_get_sample_rate_div(); */
/*     if (dlpf_cfg == 0 || dlpf_cfg == 7) { */
/*         gyro_rate = 8000; */
/*     } else if (dlpf_cfg >= 1 || dlpf_cfg <= 6) { */
/*         gyro_rate = 1000; */
/*     } else { */
/*         return -2; */
/*     } */
/*  */
/*     #<{(| calculate sample rate |)}># */
/*     return gyro_rate / (1 + sample_divider); */
/* } */
/*  */
/* int8_t mpu9250_get_sample_rate_div(void) */
/* { */
/*     char data[1]; */
/*     int retval; */
/*  */
/*     #<{(| get sample rate |)}># */
/*     data[0] = 0x00; */
/*     i2c_set_slave(MPU9250_ADDRESS); */
/*     retval = i2c_read_bytes(MPU9250_RA_SMPLRT_DIV, data, 1); */
/*     if (retval != 0) { */
/*         return -1; */
/*     } */
/*  */
/*     return data[0]; */
/* } */
/*  */
/* int8_t mpu9250_set_sample_rate_div(int8_t div) */
/* { */
/*     char data[1]; */
/*     int retval; */
/*  */
/*     #<{(| set sample rate divider |)}># */
/*     data[0] = div; */
/*     i2c_set_slave(MPU9250_ADDRESS); */
/*     retval = i2c_write_bytes(MPU9250_RA_SMPLRT_DIV, data, 1); */
/*     if (retval != 0) { */
/*         return -1; */
/*     } */
/*  */
/*     return 0; */
/* } */
/*  */
/* int8_t mpu9250_get_gyro_range(void) */
/* { */
/*     char data[1]; */
/*     int retval; */
/*  */
/*     #<{(| get gyro config |)}># */
/*     data[0] = 0x00; */
/*     i2c_set_slave(MPU9250_ADDRESS); */
/*     retval = i2c_read_bytes(MPU9250_RA_GYRO_CONFIG, data, 1); */
/*     if (retval != 0) { */
/*         return -1; */
/*     } */
/*  */
/*     #<{(| get gyro range bytes |)}># */
/*     data[0] = (data[0] >> 3) & 0b00000011; */
/*  */
/*     return data[0]; */
/* } */
/*  */
/* int8_t mpu9250_set_gyro_range(int8_t range) */
/* { */
/*     char data[1]; */
/*     uint8_t retval; */
/*  */
/*     #<{(| pre-check |)}># */
/*     if (range > 3 || range < 0) { */
/*         return -2; */
/*     } */
/*  */
/*     #<{(| set sample rate |)}># */
/*     data[0] = range << 3; */
/*     i2c_set_slave(MPU9250_ADDRESS); */
/*     retval = i2c_write_bytes(MPU9250_RA_GYRO_CONFIG, data, 1); */
/*     if (retval != 0) { */
/*         return -1; */
/*     } */
/*  */
/*     return 0; */
/* } */
/*  */
/* int8_t mpu9250_get_accel_range(void) */
/* { */
/*     char data[1]; */
/*     uint8_t retval; */
/*  */
/*     #<{(| get accel config |)}># */
/*     data[0] = 0x00; */
/*     i2c_set_slave(MPU9250_ADDRESS); */
/*     retval = i2c_write_bytes(MPU9250_RA_ACCEL_CONFIG, data, 1); */
/*     if (retval != 0) { */
/*         return -1; */
/*     } */
/*  */
/*     #<{(| get accel range bytes |)}># */
/*     data[0] = (data[0] >> 3) & 0b00000011; */
/*  */
/*     return data[0]; */
/* } */
/*  */
/* int8_t mpu9250_set_accel_range(int8_t range) */
/* { */
/*     char data[1]; */
/*     uint8_t retval; */
/*  */
/*     #<{(| pre-check |)}># */
/*     if (range > 3 || range < 0) { */
/*         return -2; */
/*     } */
/*  */
/*     #<{(| set sample rate |)}># */
/*     data[0] = range << 3; */
/*     i2c_set_slave(MPU9250_ADDRESS); */
/*     retval = i2c_write_bytes(MPU9250_RA_ACCEL_CONFIG, data, 2); */
/*     if (retval != 0) { */
/*         return -1; */
/*     } */
/*  */
/*     return 0; */
/* } */
/*  */
/* void mpu9250_info(struct mpu9250_data *data) */
/* { */
/*     printf("gyro sensitivity: %f\n", data->gyro->sensitivity); */
/*     printf("gyro offset_x: %f\n", data->gyro->offset_x); */
/*     printf("gyro offset_y: %f\n", data->gyro->offset_y); */
/*     printf("gyro offset_z: %f\n", data->gyro->offset_z); */
/*     printf("\n"); */
/*     printf("accel sensitivity: %f\n", data->accel->sensitivity); */
/*     printf("accel offset_x: %f\n", data->accel->offset_x); */
/*     printf("accel offset_y: %f\n", data->accel->offset_y); */
/*     printf("accel offset_z: %f\n", data->accel->offset_z); */
/*     printf("\n"); */
/*     printf("sample rate: %d\n", data->sample_rate); */
/*     printf("\n"); */
/* } */
/*  */
/* int8_t mpu9250_record_data(FILE *output_file, struct mpu9250_data *data) */
/* { */
/*     fprintf(output_file, "%f,", data->gyro->x); */
/*     fprintf(output_file, "%f,", data->gyro->y); */
/*     fprintf(output_file, "%f,", data->gyro->z); */
/*     fprintf(output_file, "%f,", data->gyro->pitch); */
/*     fprintf(output_file, "%f,", data->gyro->roll); */
/*  */
/*     fprintf(output_file, "%f,", data->accel->x); */
/*     fprintf(output_file, "%f,", data->accel->y); */
/*     fprintf(output_file, "%f,", data->accel->z); */
/*     fprintf(output_file, "%f,", data->accel->pitch); */
/*     fprintf(output_file, "%f,", data->accel->roll); */
/*  */
/*     fprintf(output_file, "%f,", data->pitch); */
/*     fprintf(output_file, "%f\n", data->roll); */
/*  */
/*     return 0; */
/* } */
