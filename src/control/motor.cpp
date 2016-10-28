#include "piq/control/motor.hpp"


namespace piq {
namespace control {

MotorController::MotorController(void)
{


}

int MotorController::configure(void)
{
    float duty_min;
    float duty_max;

    /* calculate min / max duty cycle */
    /* a typical ESC expects a pulse width between 1 to 2ms */
    this->frequency = 400;
    duty_min = (0.001 / (1.0 / (float) this->frequency));
    duty_max = (0.002 / (1.0 / (float) this->frequency));

    /* calculate PCA9685 motor bounds - out of 4095 (12bit range) */
    this->min = (int) 4096.0 * duty_min;
    this->max = (int) 4096.0 * duty_max;
    this->range = this->max - this->min;

    /* set motor defaults */
    this->motor_1 = 0.0f;
    this->motor_2 = 0.0f;
    this->motor_3 = 0.0f;
    this->motor_4 = 0.0f;

    /* setup pca9685 */
    pwm = comm::PCA9685();
    pwm.configure(this->frequency);
    sleep(1);
    pwm.setAllPWM(this->min);

    return 0;
}

}  // end of control
}  // end of piq
