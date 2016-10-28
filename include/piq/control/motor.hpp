#ifndef __MOTOR_CONTROLLER_HPP__
#define __MOTOR_CONTROLLER_HPP__

#include <stdint.h>

#include "piq/comm/pca9685.hpp"

namespace piq {
namespace control {

class MotorController
{
public:
    comm::PCA9685 pwm;

    int frequency;
    int min;
    int max;
    int range;

    float throttle;
    float motor_1;
    float motor_2;
    float motor_3;
    float motor_4;

    MotorController(void);
    int configure(void);
    void calibrate(void);
    void setThrottles(void);
    void throttleIncrement(float inc);
    void throttleDecrement(float dec);
};

}  // end of control
}  // end of piq
#endif
