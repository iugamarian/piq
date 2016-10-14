#ifndef __ESC_H__
#define __ESC_H__

#include <stdlib.h>

#include "piq/piq.h"
#include "piq/config.h"
#include "piq/imu/imu.h"
#include "piq/motor/pca9685.h"
#include "piq/controller/pid.h"


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
