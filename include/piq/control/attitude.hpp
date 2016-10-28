#ifndef __PIQ_ATTITUDE_CONTROLLER_HPP__
#define __PIQ_ATTITUDE_CONTROLLER_HPP__

#include <Eigen/Dense>

#include "piq/util.hpp"
#include "piq/control/pid.hpp"


namespace piq {
namespace control {

class AttitudeController
{
public:
    PID roll_controller;
    PID pitch_controller;
    PID yaw_controller;
    PID thrust_controller;

    AttitudeController(void);
    Eigen::VectorXd calculate(
        Eigen::VectorXd setpoints,
        Eigen::VectorXd actual,
        double dt
    );
};

}  // end of control namespace
}  // end of piq namespace
#endif
