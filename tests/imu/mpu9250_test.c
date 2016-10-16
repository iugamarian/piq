#include "munit.h"
#include "piq/imu/mpu9250.h"


/* #<{(| TESTS |)}># */
/* int test_mpu9250_setup(void); */
/* int test_mpu9250_ping(void); */
/* void test_suite(void); */
/*  */
/*  */
/* int test_mpu9250_setup(void) */
/* { */
/*     struct mpu9250 sensor; */
/*  */
/*     mpu9250_setup(&sensor); */
/*  */
/*     return 0; */
/* } */
/*  */
/* int test_mpu9250_ping(void) */
/* { */
/*     i2c_setup(); */
/*     mpu9250_ping(); */
/*     return 0; */
/* } */
/*  */
/* void test_suite() */
/* { */
/*     mu_add_test(test_mpu9250_setup); */
/*     mu_add_test(test_mpu9250_ping); */
/* } */

void test_suite(void) {}
mu_run_tests(test_suite)
