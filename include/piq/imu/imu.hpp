#ifndef __IMU_HPP__
#define __IMU_HPP__


#include <stdint.h>
#include "piq/imu/mpu6050.hpp"
#include "piq/imu/mpu9250.hpp"


namespace piq {
namespace imu {

class Gyroscope
{
public:
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

class Accelerometer
{
public:
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

class Magnetometer
{
public:
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

class IMU
{
public:
    Gyroscope gyro;
    Accelerometer accel;
    Magnetometer mag;

    float temperature;
    float pitch;
    float roll;

    float pitch_offset;
    float roll_offset;

    clock_t last_updated;
    float sample_rate;

    IMU(void);
};

}  // eof imu
}  // eof piq
#endif
