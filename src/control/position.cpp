#include "piq/control/position.hpp"


namespace piq {
namespace control {

PositionController::PositionController(void)
{
    this->x_controller = PID(2.0, 0.01, 0.8);
    this->y_controller = PID(2.0, 0.01, 0.8);
}

Eigen::VectorXd PositionController::calculate(
    Eigen::VectorXd setpoints,
    Eigen::VectorXd measured,
    double yaw,
    double dt)
{
    double r, p, y, t;
    Eigen::VectorXd outputs(4);

    // roll, pitch, yaw and thrust
    r = -this->y_controller.calculate(setpoints(1), measured(1), dt);
    p = this->x_controller.calculate(setpoints(0), measured(0), dt);
    y = yaw;
    t = setpoints(2);

    // outputs
    outputs(0) = r;
    outputs(1) = p;
    outputs(2) = y;
    outputs(3) = t;

    // limit roll, pitch and yaw
    for (int i = 0; i < 3; i++) {
        if (outputs(i) > deg2rad(30)) {
            outputs(i) = deg2rad(30);
        } else if (outputs(i) < deg2rad(-30)) {
            outputs(i) = deg2rad(-30);
        }
    }

    return outputs;
}

}  // end of control namespace
}  // end of piq namespace
