#include "piq/imu/imu.hpp"


namespace piq {
namespace imu {

IMU::IMU(void)
{
    this->gyro.offset_x = 0.0f;
    this->gyro.offset_y = 0.0f;
    this->gyro.offset_z = 0.0f;
    this->gyro.pitch = 0.0f;
    this->gyro.roll = 0.0f;

    this->accel.offset_x = 0.0f;
    this->accel.offset_y = 0.0f;
    this->accel.offset_z = 0.0f;
    this->accel.pitch = 0.0f;
    this->accel.roll = 0.0f;

    this->pitch = 0.0f;
    this->roll = 0.0f;
}

}  // end of imu namespace
}  // end of piq namespace
