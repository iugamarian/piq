#include "munit.h"
#include "utils.h"
#include "control.h"


/* PROTOTYPES */
int test_pid_setup_and_destroy(void);
void test_suite(void);


int test_pid_setup_and_destroy()
{
    struct pid *p;
    float zero;

    p = pid_setup(0.0f, 0.0f, 0.0f, 0.0f);
    zero = 0.0f;
    mu_check(fltcmp(&p->setpoint, &zero) == 0);
    mu_check(fltcmp(&p->k_p, &zero) == 0);
    mu_check(fltcmp(&p->k_i, &zero) == 0);
    mu_check(fltcmp(&p->k_d, &zero) == 0);
    mu_check(fltcmp(&p->dead_zone, &zero) == 0);
    mu_check(fltcmp(&p->bound_min, &zero) == 0);
    mu_check(fltcmp(&p->bound_max, &zero) == 0);
    pid_destroy(p);

    return 0;
}

void test_suite()
{
    mu_add_test(test_pid_setup_and_destroy);
}

mu_run_tests(test_suite)
