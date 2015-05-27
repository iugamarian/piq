#include "main.h"


int i2c_setup(void)
{
    bcm2835I2CReasonCodes retval;

    retval = bcm2835_init();
    if (retval == 0) {
        printf("%s\n", I2C_INIT_FAILED);
        return -1;
    }
    bcm2835_i2c_begin();

    return 0;
}

void i2c_teardown(void)
{
    bcm2835_i2c_end();
}

int mpu6050_setup(void)
{
    char buf[2];

    /* setup */
    bcm2835_i2c_setSlaveAddress(MPU6050_ADDRESS);

    /* POWER MANAGEMENT */
    buf[0] = MPU6050_RA_PWR_MGMT_1;
    buf[1] = 0x00;
    bcm2835_i2c_write(buf, 2);

    return 0;
}

void mpu6050_ping(void)
{
    char buf[1];
    char value[1];

    /* setup */
    buf[0] = MPU6050_RA_WHO_AM_I;
    value[0] = 0x00;

    /* print mpu6050 address */
    bcm2835_i2c_read_register_rs(buf, value, 1);
    printf("MPU6050 ADDRESS: 0x%02X\n", value[0]);
}

int mpu6050_data(struct mpu6050_data *data)
{
    char buf[1];
    char sensor_data[14];
    bcm2835I2CReasonCodes retval;

    /* setup */
    buf[0] = MPU6050_RA_ACCEL_XOUT_H;
    memset(sensor_data, 0, 14);

    /* read sensor data */
    retval = bcm2835_i2c_write_read_rs(buf, 1, sensor_data, 14);
    if (retval != BCM2835_I2C_REASON_OK) {
        return -1;
    }

    /* accelerometer */
    data->accel.x = (sensor_data[0] << 8) | (sensor_data[1]);
    data->accel.y = (sensor_data[2] << 8) | (sensor_data[3]);
    data->accel.z = (sensor_data[4] << 8) | (sensor_data[5]);

    /* temperature */
    data->temp = (sensor_data[6] << 8) | (sensor_data[7]);

    /* gyroscope */
    data->gyro.x = (sensor_data[8] << 8) | (sensor_data[9]);
    data->gyro.y = (sensor_data[10] << 8) | (sensor_data[10]);
    data->gyro.z = (sensor_data[12] << 8) | (sensor_data[13]);

    return 0;
}

size_t mpu6050_data_print(struct mpu6050_data *data, size_t prev_length)
{
    size_t i;
    char buf[500];
    size_t length;

    /* for (i = 0; i < prev_length; i++) { */
    /*     printf("\b \b"); */
    /* } */

    sprintf(buf, "gyro_x: %d", data->gyro.x);
    length = strlen(buf);
    printf("%s", buf);

    return length;
}

int main(void)
{
    struct mpu6050_data data;
    size_t length;

    /* setup */
    i2c_setup();
    mpu6050_setup();

    /* read values */
    printf("\n");
    length = 0;
    while (1) {
        mpu6050_data(&data);
        length = mpu6050_data_print(&data, length);
    }

    /* clean up */
    i2c_teardown();

    return 0;
}
