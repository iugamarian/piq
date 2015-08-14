#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "utils.h"


/* STRUCTURES */
struct config
{
    /* mcc config */
    char *mcc_host;
    int mcc_port;

    /* pitch pid config */
    int pitch_sample_rate;
    float pitch_k_p;
    float pitch_k_i;
    float pitch_k_d;
    float pitch_min;
    float pitch_max;

    /* roll pid config */
    int roll_sample_rate;
    float roll_k_p;
    float roll_k_i;
    float roll_k_d;
    float roll_min;
    float roll_max;

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
