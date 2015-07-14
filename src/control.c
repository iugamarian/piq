#include "control.h"


struct esc *esc_setup(void)
{
    struct esc *e;
    float duty_min;
    float duty_max;

    /* setup */
    e = malloc(sizeof(struct esc));

    /* calculate min / max duty cycle */
    e->frequency = 400;
    duty_min = (0.001 / (1.0 / (float) e->frequency));
    duty_max = (0.002 / (1.0 / (float) e->frequency));

    /* calculate PCA9685 motor bounds - out of 4095 */
    e->min = (int) 4096.0 * duty_min;
    e->max = (int) 4096.0 * duty_max;

    log_info("frequency: %d", e->frequency);
    log_info("duty min: %f", duty_min);
    log_info("duty max: %f", duty_max);
    log_info("min: %d", e->min);
    log_info("max: %d", e->max);

    /* set motor defaults */
    e->motor_1 = e->min + 100;
    e->motor_2 = e->min + 100;
    e->motor_3 = e->min + 100;
    e->motor_4 = e->min + 100;

    /* setup pca9685 */
    pca9685_setup(e->frequency);
    sleep(1);

    return e;
}

void esc_calibrate(struct esc *e)
{
    log_info("calibrate high");
    pca9685_set_all_pwm(e->max - 100);
    sleep(3);

    log_info("calibrate low");
    pca9685_set_all_pwm(e->min + 100);
    sleep(3);
}

void esc_set_throttles(struct esc *e)
{
    pca9685_set_pwm(0, e->motor_1);
    pca9685_set_pwm(1, e->motor_2);
    pca9685_set_pwm(2, e->motor_3);
    pca9685_set_pwm(3, e->motor_4);
}
