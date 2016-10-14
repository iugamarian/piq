#include "munit.h"
#include "comm/i2c.h"


/* TESTS */
int test_i2c_setup(void);
void test_suite(void);


int test_i2c_setup(void)
{
    struct i2c conn;

    i2c_setup(&conn);

    return 0;
}

void test_suite()
{
    mu_add_test(test_i2c_setup);
}

mu_run_tests(test_suite)
