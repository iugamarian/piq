#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "piq.h"
#include "pca9685.h"


/* STRUCTURES */
struct pid
{
    float setpoint;
    float prev_error;
    float output;

    float k_p;
    float k_i;
    float k_d;

    float integral_error;
    float derivative_error;

    float dead_zone;
    float bound_min;
    float bound_max;
    clock_t last_updated;
};

struct esc
{
    int frequency;
    int min;
    int max;
    int range;

    float throttle;
    float motor_1;
    float motor_2;
    float motor_3;
    float motor_4;

    struct pid *pitch_pid;
    struct pid *roll_pid;
};


/* FUNCTIONS */
struct pid *pid_setup(float setpoint, float k_p, float k_i, float k_d);
void pid_destroy(void *target);
int pid_calculate(struct pid *p, float actual);
int pid_precheck(struct pid *p);
struct esc *esc_setup(void);
void esc_destroy(void *target);
void esc_calibrate(struct esc *e);
void esc_set_throttles(struct esc *e);

#endif
