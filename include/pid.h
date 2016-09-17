#ifndef __PID_H__
#define __PID_H__

#include <stdlib.h>
#include <math.h>
#include <sys/timeb.h>

#include "config.h"


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


/* FUNCTIONS */
struct pid *pid_setup(struct pid_config *c);
void pid_destroy(void *target);
int pid_calculate(struct pid *p, float actual);


#endif
