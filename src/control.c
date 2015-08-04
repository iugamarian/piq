#include "control.h"


struct esc *esc_setup(void)
{
    struct esc *e;
    float duty_min;
    float duty_max;

    /* setup */
    e = malloc(sizeof(struct esc));
    e->pitch_pid = pid_setup(0.0f, 0.0f, 0.0f, 0.0f);
    e->roll_pid = pid_setup(0.0f, 0.0f, 0.0f, 0.0f);

    /* calculate min / max duty cycle */
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
    esc_set_throttles(e);

    return e;
}

void esc_destroy(void *target)
{
    struct esc *e;

    e = target;

    /* set throttle to 0% */
    pca9685_set_pwm(0, e->min);
    pca9685_set_pwm(1, e->min);
    pca9685_set_pwm(2, e->min);
    pca9685_set_pwm(3, e->min);

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

void esc_set_throttles(struct esc *e)
{
    e->motor_1 = e->throttle + e->roll_pid->output;
    e->motor_2 = e->throttle + e->pitch_pid->output;
    e->motor_3 = e->throttle - e->roll_pid->output;
    e->motor_4 = e->throttle - e->pitch_pid->output;

    pca9685_set_pwm(0, e->min + (e->range * e->motor_1));
    pca9685_set_pwm(1, e->min + (e->range * e->motor_2));
    pca9685_set_pwm(2, e->min + (e->range * e->motor_3));
    pca9685_set_pwm(3, e->min + (e->range * e->motor_4));
}


struct pid *pid_setup(float setpoint, float k_p, float k_i, float k_d)
{
    struct pid *p;

    p = malloc(sizeof(struct pid));

    p->setpoint = setpoint;
    p->prev_error = 0.0f;
    p->output = 0.0f;

    p->k_p = k_p;
    p->k_i = k_i;
    p->k_d = k_d;

    p->integral_error = 0.0f;
    p->derivative_error = 0.0f;

    p->dead_zone = 0.0f;
    p->bound_min = 0.0f;
    p->bound_max = 0.0f;
    p->last_updated = clock();

    return p;
}

void pid_destroy(void *target)
{
    struct pid *p;

    p = target;
    free(p);
    p = NULL;
}

int pid_precheck(struct pid *p)
{
    int k_p_zero;
    int k_i_zero;
    int k_d_zero;

    /* check if k_p, k_i or k_d is 0 */
    k_p_zero = (fltcmp(p->k_p, 0.0f) == 0) ? 1 : 0;
    k_i_zero = (fltcmp(p->k_i, 0.0f) == 0) ? 1 : 0;
    k_d_zero = (fltcmp(p->k_d, 0.0f) == 0) ? 1 : 0;

    if (k_p_zero && k_i_zero && k_d_zero) {
        return -1;
    }

    return 0;
}

int pid_calculate(struct pid *p, float actual)
{
    float dt;
    float error;

    /* pre-check */
    if (pid_precheck(p) == -1){
        log_err("pid constant values are zero!");
        return -1;
    }

    /* calculate dt */
    dt = ((double) clock() - p->last_updated) / CLOCKS_PER_SEC;

    /* calculate error */
    error = p->setpoint - actual;

    /* calculate derivative and integral errors */
    if (fabs(error) > p->dead_zone) {
        p->integral_error += error * dt;
    }
    if (dt) {
        p->derivative_error = (error - p->prev_error) / dt;
    }

    /* calculate output */
    p->output = (p->k_p * error);
    p->output += (p->k_i * p->integral_error);
    p->output += (p->k_d * p->derivative_error);

    /* saturation filter */
    if (p->output > p->bound_max) {
        p->output = p->bound_max;
    } else if (p->output < p->bound_min) {
        p->output = p->bound_min;
    }

    /* update error and last_updated */
    p->prev_error = error;
    p->last_updated = clock();

    return 0;
}
