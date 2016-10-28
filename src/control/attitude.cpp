#include "piq/control/attitude.hpp"


namespace piq {
namespace control {

AttitudeController::AttitudeController(void)
{
    this->roll_controller = PID(2.0, 0.0, 0.5);
    this->pitch_controller = PID(2.0, 0.0, 0.5);
    this->yaw_controller = PID(1.0, 0.0, 0.5);
    this->thrust_controller = PID(2.0, 0.0, 1.0);
}

Eigen::VectorXd AttitudeController::calculate(
    Eigen::VectorXd setpoints,
    Eigen::VectorXd actual,
    double dt)
{
    double r, p, y, t;
    Eigen::VectorXd outputs(4);

    // roll pitch yaw
    r = this->roll_controller.calculate(setpoints(0), actual(0), dt);
    p = this->pitch_controller.calculate(setpoints(1), actual(1), dt);
    y = this->yaw_controller.calculate(setpoints(2), actual(2), dt);

    // thrust
    t = 2.5 + this->thrust_controller.calculate(setpoints(3), actual(3), dt);
    t /= fabs(cos(actual(0)) * cos(actual(1))); // adjust for roll and pitch

    // limit thrust
    if (t > 5.0) {
        t = 5.0;
    }

    // map roll, pitch, yaw and thrust to motor outputs
    outputs(0) = -p - y + t;
    outputs(1) = -r + y + t;
    outputs(2) = p - y + t;
    outputs(3) = r + y + t;

    // limit outputs
    for (int i = 0; i < 4; i++) {
        if (outputs(i) > 5.0) {
            outputs(i) = 5.0;
        } else if (outputs(i) < 0.0) {
            outputs(i) = 0.0;
        }
    }

    return outputs;
}

}  // end of control namespace
}  // end of piq namespace
