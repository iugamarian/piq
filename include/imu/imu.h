#ifndef __IMU_H__
#define __IMU_H__


#include <stdint.h>
#include "imu/mpu6050.h"
#include "imu/mpu9250.h"


/* STRUCTURES */
struct gyroscope
{
    float sensitivity;

    int16_t raw_x;
    int16_t raw_y;
    int16_t raw_z;

    float offset_x;
    float offset_y;
    float offset_z;

    float x;
    float y;
    float z;

    float pitch;
    float roll;
};

struct accelerometer
{
    float sensitivity;

    int16_t raw_x;
    int16_t raw_y;
    int16_t raw_z;

    float offset_x;
    float offset_y;
    float offset_z;

    float x;
    float y;
    float z;

    float pitch;
    float roll;
};

struct magnetometer
{
    float sensitivity;

    int16_t raw_x;
    int16_t raw_y;
    int16_t raw_z;

    float offset_x;
    float offset_y;
    float offset_z;

    float x;
    float y;
    float z;

    float heading;
};

struct imu
{
    struct gyroscope gyro;
    struct accelerometer accel;

    float temperature;
    float pitch;
    float roll;

    float pitch_offset;
    float roll_offset;

    clock_t last_updated;
    float sample_rate;
};


/* FUNCTIONS */
void imu_setup(struct imu *i);

#endif
