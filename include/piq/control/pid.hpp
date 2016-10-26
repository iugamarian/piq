#ifndef __PID_HPP__
#define __PID_HPP__

#include <stdlib.h>
#include <math.h>
#include <sys/timeb.h>


namespace piq {
namespace control {

class PID
{
    double error_prev;
    double error_sum;

    double error_p;
    double error_i;
    double error_d;

    double k_p;
    double k_i;
    double k_d;

    PID(void);
    PID(double k_p, double k_i, double k_d);
    double calculate(double setpoint, double input, double dt);
};

}  // eof comm
}  // eof piq
#endif
