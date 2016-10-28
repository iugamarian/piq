#include "piq/comm/pca9685.hpp"


namespace piq {
namespace comm {

int PCA9685::configure(int freq)
{
    char mode_1;

    /* setup */
    this->i2c = comm::I2C();
    this->i2c.setSlave(PCA9685_I2C_ADDR);

    this->reset();
    this->setAllPWM(0);

    /* configure mode 2 register */
    this->i2c.writeByte(PCA9685_MODE2, 0x04);
    this->i2c.writeByte(PCA9685_MODE1, 0x01);
    usleep(PCA9685_WAIT_MS);

    /* configure mode 1 register */
    this->i2c.readByte(PCA9685_MODE1, &mode_1);
    mode_1 = mode_1 & ~0x10;
    this->i2c.writeByte(PCA9685_MODE1, mode_1);
    usleep(PCA9685_WAIT_MS);

    /* set frequency */
    this->setPWMFrequency(freq);

    return 0;
}

void PCA9685::setPWMFrequency(int freq)
{
    float prescale;
    char mode_1_old;
    char mode_1_new;

    /* setup */
    this->i2c.setSlave(PCA9685_I2C_ADDR);

    /* set pca9685 to sleep */
    this->i2c.readByte(PCA9685_MODE1, &mode_1_old);
    mode_1_new = (mode_1_old & 0x7F) | 0x10;
    this->i2c.writeByte(PCA9685_MODE1, mode_1_new);

    /* set pwm prescaler */
    prescale = (25000000 / (4096.0 * freq)) - 1;
    prescale = floor(prescale + 0.5);
    log_info("prescale: %d", (int) prescale);
    this->i2c.writeByte(PCA9685_PRE_SCALE, (int) prescale);
    this->i2c.writeByte(PCA9685_MODE1, mode_1_old);

    /* wait for oscillator */
    usleep(PCA9685_WAIT_MS);

    /* reset */
    this->i2c.writeByte(PCA9685_MODE1, mode_1_old | (1 << 7));
}

void PCA9685::setPWM(int8_t channel, int16_t off)
{
    /* set a single PWM channel */
    this->i2c.setSlave(PCA9685_I2C_ADDR);
    this->i2c.writeByte(PCA9685_LED0_ON_L + (4 * channel), 0 & 0xFF);
    this->i2c.writeByte(PCA9685_LED0_ON_H + (4 * channel), 0 >> 8);
    this->i2c.writeByte(PCA9685_LED0_OFF_L + (4 * channel), off & 0xFF);
    this->i2c.writeByte(PCA9685_LED0_OFF_H + (4 * channel), off >> 8);
}

void PCA9685::setAllPWM(int16_t off)
{
    log_info("set all pwm: %d", off);

    /* sets a all PWM channels */
    this->i2c.setSlave(PCA9685_I2C_ADDR);
    this->i2c.writeByte(PCA9685_ALL_LED_ON_L, 0 & 0xFF);
    this->i2c.writeByte(PCA9685_ALL_LED_ON_H, 0 >> 8);
    this->i2c.writeByte(PCA9685_ALL_LED_OFF_L, off & 0xFF);
    this->i2c.writeByte(PCA9685_ALL_LED_OFF_H, off >> 8);
}

void PCA9685::reset(void)
{
    this->i2c.setSlave(0x00);
    this->i2c.writeRawByte(0x06);
    usleep(PCA9685_WAIT_MS);
}

}  // end of motor namespace
}  // end of piq namespace
