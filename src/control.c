#include "control.h"


struct motors motors_setup(void)
{
    struct motors m;
    float duty_min;
    float duty_max;

    /* calculate min / max duty cycle */
    m.frequency = 400;
    duty_min = (0.001 / (1.0 / (float) m.frequency));
    duty_max = (0.002 / (1.0 / (float) m.frequency));

    /* calculate PCA9685 motor bounds - out of 4095 */
    m.min = (int) 4096.0 * duty_min;
    m.max = (int) 4096.0 * duty_max;

    log_info("duty min: %f", duty_min);
    log_info("duty max: %f", duty_max);
    log_info("min: %d", m.min);
    log_info("max: %d", m.max);

    /* set motor defaults */
    m.motor_1 = m.min + 100;
    m.motor_2 = m.min + 100;
    m.motor_3 = m.min + 100;
    m.motor_4 = m.min + 100;

    /* setup pca9685 */
    pca9685_setup();
    pca9685_set_pwm_frequency(m.frequency);

    return m;
}

void motors_set_throttles(struct motors *m)
{
    pca9685_set_pwm(1, m->motor_1);
    pca9685_set_pwm(2, m->motor_2);
    pca9685_set_pwm(3, m->motor_3);
    pca9685_set_pwm(4, m->motor_4);
}
