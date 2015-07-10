#ifndef __CONTROL_H__
#define __CONTROL_H__


#include "pca9685.h"


/* STRUCTURES */
struct motors
{
    int frequency;
    int min;
    int max;

    int motor_1;
    int motor_2;
    int motor_3;
    int motor_4;
};


/* FUNCTIONS */
struct motors motors_setup(void);
void motors_calibrate(struct motors *m);
void motors_set_throttles(struct motors *m);



#endif
