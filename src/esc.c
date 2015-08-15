#include "esc.h"


struct esc *esc_setup(struct config *c)
{
    struct esc *e;
    float duty_min;
    float duty_max;

    /* setup */
    e = malloc(sizeof(struct esc));
    e->pitch_pid = pid_setup(&c->pitch);
    e->roll_pid = pid_setup(&c->roll);

    /* calculate min / max duty cycle */
    /* a typical ESC expects a pulse width between 1 to 2ms */
    e->frequency = 400;
    duty_min = (0.001 / (1.0 / (float) e->frequency));
    duty_max = (0.002 / (1.0 / (float) e->frequency));

    /* calculate PCA9685 motor bounds - out of 4095 (12bit range) */
    e->min = (int) 4096.0 * duty_min;
    e->max = (int) 4096.0 * duty_max;
    e->range = e->max - e->min;

    /* set motor defaults */
    e->motor_1 = 0.0f;
    e->motor_2 = 0.0f;
    e->motor_3 = 0.0f;
    e->motor_4 = 0.0f;

    /* setup pca9685 */
    pca9685_setup(e->frequency);
    sleep(1);
    pca9685_set_all_pwm(e->min);

    return e;
}

void esc_destroy(void *target)
{
    struct esc *e;

    e = target;

    /* set throttle to 0% */
    pca9685_set_all_pwm(e->min);

    /* free pids */
    pid_destroy(e->pitch_pid);
    pid_destroy(e->roll_pid);

    free(e);
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

void esc_set_throttles(struct esc *e, struct mpu6050_data *imu)
{
    pid_calculate(e->pitch_pid, imu->pitch);
    pid_calculate(e->roll_pid, imu->roll);

    e->motor_1 = e->throttle - e->roll_pid->output + e->pitch_pid->output;
    e->motor_2 = e->throttle + e->roll_pid->output + e->pitch_pid->output;
    e->motor_3 = e->throttle - e->roll_pid->output - e->pitch_pid->output;
    e->motor_4 = e->throttle + e->roll_pid->output - e->pitch_pid->output;

    pca9685_set_pwm(0, e->min + (e->range * e->motor_1));
    pca9685_set_pwm(1, e->min + (e->range * e->motor_2));
    pca9685_set_pwm(2, e->min + (e->range * e->motor_3));
    pca9685_set_pwm(3, e->min + (e->range * e->motor_4));
}

void esc_throttle_increment(struct esc *e, float inc)
{
    log_info("throttle up");
    e->throttle += inc;
}

void esc_throttle_decrement(struct esc *e, float dec)
{
    log_info("throttle down");
    e->throttle -= dec;
}
