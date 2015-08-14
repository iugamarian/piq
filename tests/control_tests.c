#include "munit.h"
#include "utils.h"
#include "control.h"


/* PROTOTYPES */
int test_pid_setup_and_destroy(void);
int test_pid_calculate(void);
void test_suite(void);


int test_pid_setup_and_destroy()
{
    struct pid *p;

    p = pid_setup(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    mu_check(fltcmp(p->setpoint, 0.0f) == 0);
    mu_check(fltcmp(p->k_p, 0.0f) == 0);
    mu_check(fltcmp(p->k_i, 0.0f) == 0);
    mu_check(fltcmp(p->k_d, 0.0f) == 0);
    mu_check(fltcmp(p->dead_zone, 0.0f) == 0);
    mu_check(fltcmp(p->bound_min, 0.0f) == 0);
    mu_check(fltcmp(p->bound_max, 0.0f) == 0);
    pid_destroy(p);

    return 0;
}

int test_pid_calculate()
{
    struct pid *p;
    int retval;
    int i;

    /* test */
    p = pid_setup(0.0f, 0.1f, 0.1f, 0.05f, -90.0f, 90.0f);
    p->output = 1.0f;
    for (i = 0; i < 10; i++) {
        usleep(100000);
        retval = pid_calculate(p, p->output);
        mu_check(retval == 0);
        log_info("p->output: %f", p->output);
    }
    pid_destroy(p);

    return 0;
}

void test_suite()
{
    /* mu_add_test(test_pid_setup_and_destroy); */
    mu_add_test(test_pid_calculate);
}

mu_run_tests(test_suite)
