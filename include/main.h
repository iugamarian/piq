#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>

#include <bcm2835.h>


/* STRUCTS */
struct gyro
{
    uint16_t x;
    uint16_t y;
    uint16_t z;
};

struct accel
{
    uint16_t x;
    uint16_t y;
    uint16_t z;
};


/* ERROR MESSAGES */
#define I2C_INIT_FAILED "failed to initialize I2C!"


/* FUNCTIONS */


#endif
