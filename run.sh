#!/bin/sh
make
valgrind ./bin/config_tests
# valgrind ./bin/control_tests
# ./bin/control_tests

# ./bin/piq
# python scripts/plot.py
# python scripts/py_pwm.py
# python scripts/pwm_helper.py
