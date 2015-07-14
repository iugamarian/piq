#ifndef __CONTROL_H__
#define __CONTROL_H__


#include "pca9685.h"


/* STRUCTURES */
struct esc
{
    int frequency;
    int min;
    int max;

    int16_t motor_1;
    int16_t motor_2;
    int16_t motor_3;
    int16_t motor_4;
};


/* FUNCTIONS */
struct esc *esc_setup(void);
void esc_calibrate(struct esc *e);
void esc_set_throttles(struct esc *e);

#endif
