#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/timeb.h>

#include "piq.h"
#include "config.h"
#include "pca9685.h"
#include "mpu6050.h"


/* STRUCTURES */
struct pid
{
    int sample_rate;

    float setpoint;
    float output;

    float prev_error;
    float sum_error;

    float k_p;
    float k_i;
    float k_d;

    float dead_zone;
    float min;
    float max;
    struct timeb last_updated;
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
struct pid *pid_setup(struct pid_config *c);
void pid_destroy(void *target);
int pid_calculate(struct pid *p, float actual);
/* ESC FUNCTIONS */
struct esc *esc_setup(struct config *c);
void esc_destroy(void *target);
void esc_calibrate(struct esc *e);
void esc_set_throttles(struct esc *e, struct mpu6050_data *imu);
void esc_throttle_increment(struct esc *e, float inc);
void esc_throttle_decrement(struct esc *e, float dec);

#endif
