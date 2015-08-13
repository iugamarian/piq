#include "munit.h"
#include "config.h"

#define TEST_CONFIG "tests/data/piq.config"


/* PROTOTYPES */
int test_config_load(void);
void test_suite(void);


int test_config_load()
{
    struct config *c;

    c = config_load(TEST_CONFIG);
    config_info(c);
    config_destroy(c);

    return 0;
}

void test_suite()
{
    mu_add_test(test_config_load);
}

mu_run_tests(test_suite)
