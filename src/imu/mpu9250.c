#include "imu/mpu9250.h"


void mpu9250_setup(struct mpu9250_data *data)
{
    data->gyro.offset_x = 0.0f;
    data->gyro.offset_y = 0.0f;
    data->gyro.offset_z = 0.0f;
    data->gyro.pitch = 0.0f;
    data->gyro.roll = 0.0f;

    data->accel.offset_x = 0.0f;
    data->accel.offset_y = 0.0f;
    data->accel.offset_z = 0.0f;
    data->accel.pitch = 0.0f;
    data->accel.roll = 0.0f;

    data->pitch = 0.0f;
    data->roll = 0.0f;

    data->sample_rate = -1.0;
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
