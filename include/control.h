#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "piq.h"
#include "pca9685.h"


/* STRUCTURES */
struct esc
{
    int frequency;
    int min;
    int max;

    int8_t motor_1;
    int8_t motor_2;
    int8_t motor_3;
    int8_t motor_4;
};

struct pid
{
    float setpoint;
    float prev_error;
    float output;

    float k_p;
    float k_i;
    float k_d;

    float dead_zone;
    float bound_min;
    float bound_max;
    clock_t last_updated;
};


/* FUNCTIONS */
struct esc *esc_setup(void);
void esc_destroy(void *target);
void esc_calibrate(struct esc *e);
void esc_set_throttles(struct esc *e);
struct pid *pid_setup(float setpoint, float k_p, float k_i, float k_d);
void pid_destroy(void *target);
float pid_calculate(struct pid *p, float actual);

#endif
