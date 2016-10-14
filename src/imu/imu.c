#include "piq/imu/imu.h"


void imu_setup(struct imu *i)
{
    i->gyro.offset_x = 0.0f;
    i->gyro.offset_y = 0.0f;
    i->gyro.offset_z = 0.0f;
    i->gyro.pitch = 0.0f;
    i->gyro.roll = 0.0f;

    i->accel.offset_x = 0.0f;
    i->accel.offset_y = 0.0f;
    i->accel.offset_z = 0.0f;
    i->accel.pitch = 0.0f;
    i->accel.roll = 0.0f;

    i->pitch = 0.0f;
    i->roll = 0.0f;
}
