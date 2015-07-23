#ifndef __MAIN_H__
#define __MAIN_H__

#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#include "i2c.h"
#include "utils.h"
#include "control.h"
#include "mpu6050.h"
#include "pca9685.h"
#include "telemetry.h"


/* STRUCTURES */
struct piq
{
    struct mpu6050_data *imu;
    struct esc *motors;
};


/* FUNCTIONS */
void *mpu6050_loop(void *arg);
struct piq *piq_setup(void);
void piq_destroy(void *target);


#endif
