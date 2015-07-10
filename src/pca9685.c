#include "pca9685.h"


void pca9685_setup(void)
{
    char mode_1;

    /* setup */
    i2c_set_slave(PCA9685_I2C_ADDR);
    pca9685_set_all_pwm(0);

    /* configure mode 2 register */
    i2c_write_byte(PCA9685_MODE2, 0x04);
    i2c_write_byte(PCA9685_MODE1, 0x01);
    usleep(PCA9685_WAIT_MS);

    /* configure mode 1 register */
    i2c_read_byte(PCA9685_MODE1, &mode_1);
    mode_1 = mode_1 & ~0x10;
    i2c_write_byte(PCA9685_MODE1, mode_1);
    usleep(PCA9685_WAIT_MS);
}

void pca9685_set_pwm_frequency(int freq)
{
    float prescale;
    char mode_1_old;
    char mode_1_new;

    /* setup */
    i2c_set_slave(PCA9685_I2C_ADDR);

    /* set pca9685 to sleep */
    i2c_read_byte(PCA9685_MODE1, &mode_1_old);
    mode_1_new = (mode_1_old & 0x7F) | 0x10;
    i2c_write_byte(PCA9685_MODE1, mode_1_new);

    /* set pwm prescaler */
    prescale = (25000000 / (4096.0 * freq)) - 1;
    prescale = floor(prescale + 0.5);
    log_info("prescale: %d", (int) prescale);
    i2c_write_byte(PCA9685_PRE_SCALE, (uint8_t) prescale);
    i2c_write_byte(PCA9685_MODE1, mode_1_old);

    /* wait for oscillator */
    usleep(PCA9685_WAIT_MS);

    /* reset */
    i2c_write_byte(PCA9685_MODE1, mode_1_old | (1 << 7));
}

void pca9685_set_pwm(uint8_t channel, uint16_t off)
{
    /* setup */
    i2c_set_slave(PCA9685_I2C_ADDR);

    /* set a single PWM channel */
    i2c_set_slave(PCA9685_I2C_ADDR);
    i2c_write_byte(PCA9685_ALL_LED_ON_L + (4 * channel), 0 & 0xFF);
    i2c_write_byte(PCA9685_ALL_LED_ON_H + (4 * channel), 0 >> 8);
    i2c_write_byte(PCA9685_ALL_LED_OFF_L + (4 * channel), off & 0xFF);
    i2c_write_byte(PCA9685_ALL_LED_OFF_H + (4 * channel), off >> 8);
}

void pca9685_set_all_pwm(uint16_t off)
{
    /* setup */
    i2c_set_slave(PCA9685_I2C_ADDR);

    /* sets a all PWM channels */
    i2c_set_slave(PCA9685_I2C_ADDR);
    i2c_write_byte(PCA9685_ALL_LED_ON_L, 0 & 0xFF);
    i2c_write_byte(PCA9685_ALL_LED_ON_H, 0 >> 8);
    i2c_write_byte(PCA9685_ALL_LED_OFF_L, off & 0xFF);
    i2c_write_byte(PCA9685_ALL_LED_OFF_H, off >> 8);
}

void pca9685_reset(void)
{
    i2c_set_slave(0x00);
    i2c_write_raw_byte(0x06);
}
