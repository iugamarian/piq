#include "pca9685.h"


void pca9685_setup(void)
{
    char mode_1;

    /* configure mode 2 register */
    i2c_set_slave(PCA9685_I2C_ADDR);
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
    i2c_write_byte(PCA9685_PRE_SCALE, prescale);
    i2c_write_byte(PCA9685_MODE1, mode_1_old);

    /* wait for oscillator */
    usleep(PCA9685_WAIT_MS);

    /* reset */
    i2c_write_byte(PCA9685_MODE1, mode_1_old | (1 << 7));
}

void pca9685_set_pwm(uint8_t channel, uint8_t duty_cycle)
{
    uint16_t on;
    uint16_t off;

    /* calculate on / off counts */
    on = 0;
    off = (int16_t) 4095.0 * (duty_cycle / 100.0);

    /* setup */
    i2c_set_slave(PCA9685_I2C_ADDR);

    /* set a single PWM channel */
    i2c_set_slave(PCA9685_I2C_ADDR);
    i2c_write_byte(PCA9685_ALL_LED_ON_L + (4 * channel), on & 0xFF);
    i2c_write_byte(PCA9685_ALL_LED_ON_H + (4 * channel), on >> 8);
    i2c_write_byte(PCA9685_ALL_LED_OFF_L + (4 * channel), off & 0xFF);
    i2c_write_byte(PCA9685_ALL_LED_OFF_H + (4 * channel), off >> 8);
}

void pca9685_set_all_pwm(uint8_t duty_cycle)
{
    uint16_t on;
    uint16_t off;

    /* setup */
    i2c_set_slave(PCA9685_I2C_ADDR);

    /* calculate on / off counts */
    on = 0;
    off = (int16_t) 4096.0 * (duty_cycle / 100.0);

    /* sets a all PWM channels */
    i2c_set_slave(PCA9685_I2C_ADDR);
    i2c_write_byte(PCA9685_ALL_LED_ON_L, on & 0xFF);
    i2c_write_byte(PCA9685_ALL_LED_ON_H, on >> 8);
    i2c_write_byte(PCA9685_ALL_LED_OFF_L, off & 0xFF);
    i2c_write_byte(PCA9685_ALL_LED_OFF_H, off >> 8);
}
