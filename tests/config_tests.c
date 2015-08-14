#include "munit.h"
#include "config.h"

#define TEST_CONFIG "tests/data/piq.config"


/* PROTOTYPES */
int test_config_new_and_destroy(void);
int test_config_load(void);
void test_suite(void);

int test_config_new_and_destroy(void)
{

    struct config *c;

    c = config_new();

    /* mcc config */
    mu_check(c->mcc_host == NULL);
    mu_check(c->mcc_port == -1);

    /* pitch pid config */
    mu_check(c->pitch_sample_rate == 100);
    mu_check(fltcmp(c->pitch_k_p, 0.0f) == 0);
    mu_check(fltcmp(c->pitch_k_i, 0.0f) == 0);
    mu_check(fltcmp(c->pitch_k_d, 0.0f) == 0);
    mu_check(fltcmp(c->pitch_min, -1.0f) == 0);
    mu_check(fltcmp(c->pitch_max, 1.0f) == 0);

    /* roll pid config */
    mu_check(c->roll_sample_rate == 100);
    mu_check(fltcmp(c->roll_k_p, 0.0f) == 0);
    mu_check(fltcmp(c->roll_k_i, 0.0f) == 0);
    mu_check(fltcmp(c->roll_k_d, 0.0f) == 0);
    mu_check(fltcmp(c->roll_min, -1.0f) == 0);
    mu_check(fltcmp(c->roll_max, 1.0f) == 0);

    /* pitch & roll offset */
    mu_check(fltcmp(c->pitch_offset, 0.0f) == 0);
    mu_check(fltcmp(c->roll_offset, 0.0f) == 0);

    config_destroy(c);

    return 0;
}

int test_config_load()
{
    struct config *c;

    c = config_new();

    /* mcc config */
    config_load(c, TEST_CONFIG);
    config_info(c);

    /* pitch pid config */
    mu_check(c->pitch_sample_rate == 200);
    mu_check(fltcmp(c->pitch_k_p, 0.01f) == 0);
    mu_check(fltcmp(c->pitch_k_i, 0.01f) == 0);
    mu_check(fltcmp(c->pitch_k_d, 0.01f) == 0);
    mu_check(fltcmp(c->pitch_min, 0.01f) == 0);
    mu_check(fltcmp(c->pitch_max, 0.02f) == 0);

    /* roll pid config */
    mu_check(c->roll_sample_rate == 200);
    mu_check(fltcmp(c->roll_k_p, 0.01f) == 0);
    mu_check(fltcmp(c->roll_k_i, 0.01f) == 0);
    mu_check(fltcmp(c->roll_k_d, 0.01f) == 0);
    mu_check(fltcmp(c->roll_min, 0.01f) == 0);
    mu_check(fltcmp(c->roll_max, 0.02f) == 0);

    /* pitch & roll offset */
    mu_check(fltcmp(c->pitch_offset, 0.0f) == 0);
    mu_check(fltcmp(c->roll_offset, 0.0f) == 0);

    config_destroy(c);

    return 0;
}

void test_suite()
{
    mu_add_test(test_config_new_and_destroy);
    mu_add_test(test_config_load);
}

mu_run_tests(test_suite)
