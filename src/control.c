#include "control.h"


struct motors motors_setup(void)
{
    struct motors m;

    /* set motor defaults */
    m.duty_min = 0;
    m.duty_max = 0;

    m.motor_1 = 0;
    m.motor_2 = 0;
    m.motor_3 = 0;
    m.motor_4 = 0;

    /* setup pca9685 */
    pca9685_setup();
    pca9685_set_pwm_frequency(400);

    return m;
}

void motors_set_throttle(struct motors *m)
{
    pca9685_set_pwm(1, m->motor_1);
    pca9685_set_pwm(2, m->motor_2);
    pca9685_set_pwm(3, m->motor_3);
    pca9685_set_pwm(4, m->motor_4);
}
