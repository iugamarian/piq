#!/bin/sh
set -e
make
# valgrind ./bin/config_tests
# valgrind ./bin/pid_tests
# valgrind ./bin/mpu9250_tests

# ./bin/piq
# python scripts/plot.py
# python scripts/py_pwm.py
# python scripts/pwm_helper.py
