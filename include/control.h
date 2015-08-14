#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "piq.h"
#include "config.h"
#include "pca9685.h"
#include "mpu6050.h"


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
/* PID FUNCTIONS */
struct pid *pid_setup(
    float setpoint,
    float k_p,
    float k_i,
    float k_d,
    float bound_min,
    float bound_max
);
void pid_destroy(void *target);
int pid_precheck(struct pid *p);
int pid_calculate(struct pid *p, float actual);
/* ESC FUNCTIONS */
struct esc *esc_setup(struct config *c);
void esc_destroy(void *target);
void esc_calibrate(struct esc *e);
void esc_set_throttles(struct esc *e, struct mpu6050_data *imu);
void esc_throttle_increment(struct esc *e, float inc);
void esc_throttle_decrement(struct esc *e, float dec);

#endif
