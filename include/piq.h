#ifndef __MAIN_H__
#define __MAIN_H__

#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#include "util.h"
#include "imu/imu.h"
#include "imu/mpu6050.h"
#include "motor/esc.h"
#include "motor/pca9685.h"
#include "comm/comm.h"
#include "comm/i2c.h"
#include "config.h"


/* STATES */
#define PIQ_STOP 0
#define PIQ_RUN 1


/* STRUCTURES */
struct piq
{
    int state;
    struct config *config;
    struct imu imu;
    struct esc *motors;
};


/* FUNCTIONS */
void piq_setup(struct piq *p);


#endif
