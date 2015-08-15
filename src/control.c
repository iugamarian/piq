#include "control.h"


/* PID FUNCTIONS */
struct pid *pid_setup(struct pid_config *c)
{
    struct pid *p;

    p = malloc(sizeof(struct pid));

    p->sample_rate = (c->sample_rate == NAN) ? 100 : c->sample_rate;

    p->setpoint = (c->setpoint == NAN) ? 0 : c->setpoint;
    p->output = 0.0f;

    p->prev_error = 0.0f;
    p->sum_error = 0.0f;

    p->k_p = (c->k_p == NAN) ? 0.0f : c->k_p;
    p->k_i = (c->k_i == NAN) ? 0.0f : c->k_i;
    p->k_d = (c->k_d == NAN) ? 0.0f : c->k_d;

    p->dead_zone = c->dead_zone;
    p->min = c->min;
    p->max = c->max;
    ftime(&p->last_updated);

    return p;
}

void pid_destroy(void *target)
{
    struct pid *p;

    p = target;
    free(p);
    p = NULL;
}

int pid_calculate(struct pid *p, float input)
{
    int dt;
    float error;
    struct timeb now;

    /* calculate dt - in miliseconds */
    ftime(&now);
    dt = (float) (
        1000.0 *
        (now.time - p->last_updated.time) +
        (now.millitm - p->last_updated.millitm)
    );

    /* calculate output */
    if (dt >= p->sample_rate) {
        /* calculate errors */
        error = p->setpoint - input;
        if (fabs(error) > p->dead_zone) {
            p->sum_error += error * (float) (dt / 1000.0);
        }

        /* calculate output */
        p->output = (p->k_p * error);
        p->output += (p->k_i * p->sum_error);
        p->output -= (p->k_d * (input - p->prev_error));

        /* limit boundaries */
        if (p->output > p->max) {
            p->output = p->max;
        } else if (p->output < p->min) {
            p->output = p->min;
        }

        /* update error and last_updated */
        p->prev_error = error;
        ftime(&p->last_updated);
    }

    return 0;
}


/* ESC FUNCTIONS */
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
