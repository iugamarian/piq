#include "piq/imu/mpu9250.h"


void mpu9250_setup(struct mpu9250 *sensor, struct i2c *conn)
{
    /* setup */
    sensor->conn = conn;
    i2c_set_slave(sensor->conn, MPU9250_ADDRESS);

    /* set initial values */
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

    sensor->pitch = 0.0f;
    sensor->roll = 0.0f;

    sensor->sample_rate = -1.0;
}

int8_t mpu9250_ping(struct mpu9250 *sensor)
{
    char data;

    /* print mpu9250 address */
    i2c_set_slave(sensor->conn, MPU9250_ADDRESS);
    i2c_read_byte(sensor->conn, MPU9250_WHO_AM_I, &data);
    printf("MPU9250 ADDRESS: 0x%02X\n", data);

    return data;
}

int8_t mpu9250_set_gyro_scale(struct mpu9250 *sensor, int8_t scale)
{
    char data;
    uint8_t retval;

    /* pre-check */
    if (scale > 3 || scale < 0) {
        return -2;
    }

    /* set gyro scale */
    data = scale << 3;
    i2c_set_slave(sensor->conn, MPU9250_ADDRESS);
    retval = i2c_write_byte(sensor->conn, MPU9250_GYRO_CONFIG, data);
    if (retval != 0) {
        return -1;
    }

    return 0;
}

int8_t mpu9250_get_gyro_scale(struct mpu9250 *sensor)
{
    char data;
    uint8_t retval;

    /* get gyro scale */
    data = 0x00;
    i2c_set_slave(sensor->conn, MPU9250_ADDRESS);
    retval = i2c_read_byte(sensor->conn, MPU9250_GYRO_CONFIG, &data);
    if (retval != 0) {
        return -1;
    }

    /* get gyro scale bytes */
    data = (data >> 3) & 0b00000011;

    return data;
}

int8_t mpu9250_set_accel_scale(struct mpu9250 *sensor, int8_t scale)
{
    char data;
    uint8_t retval;

    /* pre-check */
    if (scale > 3 || scale < 0) {
        return -2;
    }

    /* set accel scale */
    data = scale << 3;
    i2c_set_slave(sensor->conn, MPU9250_ADDRESS);
    retval = i2c_write_byte(sensor->conn, MPU9250_ACCEL_CONFIG, data);
    if (retval != 0) {
        return -1;
    }

    return 0;
}

int8_t mpu9250_get_accel_scale(struct mpu9250 *sensor)
{
    char data;
    uint8_t retval;

    /* get accel scale */
    data = 0x00;
    i2c_set_slave(sensor->conn, MPU9250_ADDRESS);
    retval = i2c_read_byte(sensor->conn, MPU9250_ACCEL_CONFIG, &data);
    if (retval != 0) {
        return -1;
    }

    /* get accel scale bytes */
    data = (data >> 3) & 0b00000011;

    return data;
}

int8_t mpu9250_set_accel_fchoice(struct mpu9250 *sensor, int8_t fchoice)
{
    char data;
    uint8_t retval;

    /* set accel scale */
    data = fchoice << 3;
    i2c_set_slave(sensor->conn, MPU9250_ADDRESS);
    retval = i2c_write_byte(sensor->conn, MPU9250_ACCEL_CONFIG2, data);
    if (retval != 0) {
        return -1;
    }

    return 0;
}

int8_t mpu9250_get_accel_fchoice(struct mpu9250 *sensor)
{
    char data;
    uint8_t retval;

    /* get accel scale */
    data = 0x00;
    i2c_set_slave(sensor->conn, MPU9250_ADDRESS);
    retval = i2c_read_byte(sensor->conn, MPU9250_ACCEL_CONFIG2, &data);
    if (retval != 0) {
        return -1;
    }

    /* get accel scale bytes */
    data = (data >> 3) & 0b00000001;

    return data;
}
