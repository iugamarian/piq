#include "pca9685.h"


static void pca9685_config_mode1_register()
{
    /* get mode1 register settings */
    mode1 = i2c_read_byte(PCA9685_MODE1);

    /* ALLCALL: set PCA9685 to respond to LED All Call I2C-bus address */
    model |= 1 << 0;

    /* SLEEP: set normal mode */
    model |= 1 << 4;

    /* set mode1 register settings */
    i2c_write_byte(PCA9685_MODE1, model);

    /* wait for oscillator */
    usleep(PCA9685_WAIT_MS);
}

static void pca9685_config_mode2_register()
{
    /* get mode2 register settings */
    mode2 = i2c_read_byte(PCA9685_MODE2);

    /* OUTDRV: LEDs set to totem pole structure */
    mode2 |= 1 << 2;

    /* set mode2 register settings */
    i2c_write_byte(PCA9685_MODE2, mode2);

    /* wait for oscillator */
    usleep(PCA9685_WAIT_MS);
}

void pca9685_setup(void)
{
    uint8_t mode1;
    uint8_t mode2;

    /* switch off all pwm channels */
    pca9685_set_all_pwm(0, 0);

    /* configure mode2 and mode1 registers: order is important here! */
    pca9685_config_mode2_register();
    pca9685_config_mode_register();
}

void pca9685_set_pwm_frequency(uint8_t freq)
{
    float prescale;
    uint8_t mode1_old;
    uint8_t mode1_new;

    /* set pca9685 to sleep */
    mode1_old = i2c_read_byte(PCA9685_MODE1);
    mode1_new = (mode1_old & 0x7F) | 0x10;
    i2c_write_byte(PCA9685_MODE1, mode1_new);

    /* set pwm prescaler */
    prescale = (25000000 / (4996.0 / float(freq))) - 1;
    prescale = floor(prescale + 0.5);
    i2c_write_byte(PCA9685_PRE_SCALE, prescale);
    i2c_write_byte(PCA9685_MODE1, mode1_old);

    /* wait for oscillator */
    usleep(PCA9685_WAIT_MS);

    /* reset */
    i2c_write_byte(PCA9685_MODE1, mode1_old | (1 << 7));
}

void pca9685_set_pwm(uint8_t channel, uint8_t duty_cycle)
{
    uint16_t on;
    uint16_t off;

    /* calculate on / off counts */
    on = 4095 * (duty_cycle / 100);
    off = 4095 - on;

    /* set a single PWM channel */
    i2c_set_slave(PCA9685_I2C_ADDR);
    i2c_write_byte(PCA9685_ALL_LED0_ON_L + (4 * channel), on & 0xFF);
    i2c_write_byte(PCA9685_ALL_LED0_ON_H + (4 * channel), on >> 8);
    i2c_write_byte(PCA9685_ALL_LED0_OFF_L + (4 * channel), off & 0xFF);
    i2c_write_byte(PCA9685_ALL_LED0_OFF_H + (4 * channel), off >> 8);
}

void pca9685_set_all_pwm(uint8_t duty_cycle)
{
    /* calculate on / off counts */
    on = 4095 * (duty_cycle / 100);
    off = 4095 - on;

    /* sets a all PWM channels */
    i2c_set_slave(PCA9685_I2C_ADDR);
    i2c_write_byte(PCA9685_ALL_LED_ON_L, on & 0xFF);
    i2c_write_byte(PCA9685_ALL_LED_ON_H, on >> 8);
    i2c_write_byte(PCA9685_ALL_LED_OFF_L, off & 0xFF);
    i2c_write_byte(PCA9685_ALL_LED_OFF_H, off >> 8);
}
