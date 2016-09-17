#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "utils.h"


/* STRUCTURES */
struct pid_config
{
    int sample_rate;
    float setpoint;

    float k_p;
    float k_i;
    float k_d;

    float dead_zone;
    float min;
    float max;
};

struct config
{
    /* mcc config */
    char *mcc_host;
    int mcc_port;

    /* pitch pid config */
    struct pid_config pitch;
    struct pid_config roll;

    /* pitch & roll offset */
    float pitch_offset;
    float roll_offset;
};


/* FUNCTIONS */
struct config *config_new(void);
void config_destroy(void *target);
struct config *config_setup(void);
int config_load(struct config *c, const char *file_path);
void config_info(struct config *c);

#endif
