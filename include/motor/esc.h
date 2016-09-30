#ifndef __ESC_H__
#define __ESC_H__

#include <stdlib.h>

#include "piq.h"
#include "config.h"
#include "imu/imu.h"
#include "motor/pca9685.h"
#include "controller/pid.h"


/* STRUCTURES */
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
struct esc *esc_setup(struct config *c);
void esc_destroy(void *target);
void esc_calibrate(struct esc *e);
void esc_set_throttles(struct esc *e, struct imu *imu);
void esc_throttle_increment(struct esc *e, float inc);
void esc_throttle_decrement(struct esc *e, float dec);


#endif
