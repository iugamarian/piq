#ifndef __MAIN_H__
#define __MAIN_H__

#include <unistd.h>
#include <fcntl.h>

#include "i2c.h"
#include "utils.h"
#include "control.h"
#include "mpu6050.h"
#include "pca9685.h"
#include "comms.h"


#define PIQ_STOP 0
#define PIQ_RUN 1


/* STRUCTURES */
struct piq
{
    int state;
    struct mpu6050_data *imu;
    struct esc *motors;
};


/* FUNCTIONS */
void *mpu6050_loop(void *arg);
struct piq *piq_setup(void);
void piq_destroy(void *target);


#endif
