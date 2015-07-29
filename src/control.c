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
    esc_set_throttles(e);

    return e;
}

void esc_destroy(void *target)
{
    struct esc *e;
    e = target;
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
    /*
        e->motor_1 = e->motor_1 + rollpid;
        e->motor_2 = e->motor_2 + pitchpid;
        e->motor_3 = e->motor_3 - rollpid;
        e->motor_4 = e->motor_4 - pitchpid;
    */

    log_info("throttle_1: %d", e->motor_1);
    log_info("throttle_2: %d", e->motor_2);
    log_info("throttle_3: %d", e->motor_3);
    log_info("throttle_4: %d", e->motor_4);

    pca9685_set_pwm(0, e->motor_1);
    pca9685_set_pwm(1, e->motor_2);
    pca9685_set_pwm(2, e->motor_3);
    pca9685_set_pwm(3, e->motor_4);
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

    p->dead_zone = 0.0f;
    p->bound_min = 0.0f;
    p->bound_max = 0.0f;

    return p;
}

void pid_destroy(void *target)
{
    struct pid *p;

    p = target;
    free(p);
    p = NULL;
}

float pid_calculate(struct pid *p, float actual)
{
    float dt;
    float error;
    float output;
    float integral;
    float derivative;
    clock_t time_now;

    /* setup */
    dt = 0.0;
    output = 0.0;
    integral = 0.0f;
    derivative = 0.0f;

    /* calculate dt */
    time_now = clock();
    dt = ((double) time_now - p->last_updated) / CLOCKS_PER_SEC;

    /* calculate error */
    error = p->setpoint - actual;

    /* calculate integral */
    if (abs(error) > p->dead_zone) {
        integral = integral * error * dt;
    }

    /* calculate derivative */
    derivative = (error - p->prev_error) / dt;

    /* calculate output */
    p->output = (p->k_p * error) + (p->k_i * integral) + (p->k_d * derivative);

    /* saturation filter */
    if (p->output > p->bound_max) {
        p->output = p->bound_max;
    } else if (output < p->bound_min) {
        p->output = p->bound_min;
    }

    /* update error and last_updated */
    p->prev_error = error;
    p->last_updated = clock();

    return output;
}
