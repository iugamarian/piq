#ifndef __PIQ_POSITION_CONTROLLER_HPP__
#define __PIQ_POSITION_CONTROLLER_HPP__

#include <Eigen/Dense>

#include "piq/util.hpp"
#include "piq/control/pid.hpp"


namespace piq {
namespace control {

class PositionController
{
public:
    PID x_controller;
    PID y_controller;

    PositionController(void);
    Eigen::VectorXd calculate(
        Eigen::VectorXd setpoints,
        Eigen::VectorXd actual,
        double yaw,
        double dt
    );
};

}  // end of control namespace
}  // end of piq namespace
#endif
