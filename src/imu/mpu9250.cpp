#include "piq/imu/mpu9250.hpp"


namespace piq {
namespace imu {

int8_t MPU9250::configure(void)
{
    /* setup */
    this->i2c = comm::I2C();
    this->i2c.setSlave(MPU9250_ADDRESS);

    /* set initial values */
    this->gyro.offset_x = 0.0f;
    this->gyro.offset_y = 0.0f;
    this->gyro.offset_z = 0.0f;
    this->gyro.pitch = 0.0f;
    this->gyro.roll = 0.0f;

    this->accel.offset_x = 0.0f;
    this->accel.offset_y = 0.0f;
    this->accel.offset_z = 0.0f;
    this->accel.pitch = 0.0f;
    this->accel.roll = 0.0f;

    this->pitch = 0.0f;
    this->roll = 0.0f;

    this->sample_rate = -1.0;

    return 0;
}

int8_t MPU9250::ping(void)
{
    char data;

    /* print mpu9250 address */
    this->i2c.setSlave(MPU9250_ADDRESS);
    this->i2c.readByte(MPU9250_WHO_AM_I, &data);
    printf("MPU9250 ADDRESS: 0x%02X\n", data);

    return data;
}

int8_t MPU9250::setGyroScale(int8_t scale)
{
    char data;
    uint8_t retval;

    /* pre-check */
    if (scale > 3 || scale < 0) {
        return -2;
    }

    /* set gyro scale */
    data = scale << 3;
    this->i2c.setSlave(MPU9250_ADDRESS);
    retval = this->i2c.writeByte(MPU9250_GYRO_CONFIG, data);
    if (retval != 0) {
        return -1;
    }

    return 0;
}

int8_t MPU9250::getGyroScale(void)
{
    char data;
    uint8_t retval;

    /* get gyro scale */
    data = 0x00;
    this->i2c.setSlave(MPU9250_ADDRESS);
    retval = this->i2c.readByte(MPU9250_GYRO_CONFIG, &data);
    if (retval != 0) {
        return -1;
    }

    /* get gyro scale bytes */
    data = (data >> 3) & 0b00000011;

    return data;
}

int8_t MPU9250::setAccelScale(int8_t scale)
{
    char data;
    uint8_t retval;

    /* pre-check */
    if (scale > 3 || scale < 0) {
        return -2;
    }

    /* set accel scale */
    data = scale << 3;
    this->i2c.setSlave(MPU9250_ADDRESS);
    retval = this->i2c.writeByte(MPU9250_ACCEL_CONFIG, data);
    if (retval != 0) {
        return -1;
    }

    return 0;
}

int8_t MPU9250::getAccelScale(void)
{
    char data;
    uint8_t retval;

    /* get accel scale */
    data = 0x00;
    this->i2c.setSlave(MPU9250_ADDRESS);
    retval = this->i2c.readByte(MPU9250_ACCEL_CONFIG, &data);
    if (retval != 0) {
        return -1;
    }

    /* get accel scale bytes */
    data = (data >> 3) & 0b00000011;

    return data;
}

int8_t MPU9250::setAccelFchoice(int8_t fchoice)
{
    char data;
    uint8_t retval;

    /* set accel scale */
    data = fchoice << 3;
    this->i2c.setSlave(MPU9250_ADDRESS);
    retval = this->i2c.writeByte(MPU9250_ACCEL_CONFIG2, data);
    if (retval != 0) {
        return -1;
    }

    return 0;
}

int8_t MPU9250::getAccelFchoice(void)
{
    char data;
    uint8_t retval;

    /* get accel scale */
    data = 0x00;
    this->i2c.setSlave(MPU9250_ADDRESS);
    retval = this->i2c.readByte(MPU9250_ACCEL_CONFIG2, &data);
    if (retval != 0) {
        return -1;
    }

    /* get accel scale bytes */
    data = (data >> 3) & 0b00000001;

    return data;
}

}  // end of imu namespace
}  // end of piq namespace
