#ifndef __MAIN_H__
#define __MAIN_H__

#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#include "piq/util.h"
#include "piq/config.h"
#include "piq/imu/imu.h"
#include "piq/imu/mpu6050.h"
#include "piq/motor/esc.h"
#include "piq/motor/pca9685.h"
#include "piq/comm/comm.h"
#include "piq/comm/i2c.h"


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
