#include "munit.h"
#include "piq/imu/mpu9250.h"


/* TESTS */
int test_mpu9250_setup(void);
int test_mpu9250_ping(void);
void test_suite(void);


int test_mpu9250_setup(void)
{
    struct i2c conn;
    struct mpu9250 sensor;

    i2c_setup(&conn);
    mpu9250_setup(&sensor, &conn);

    return 0;
}

int test_mpu9250_ping(void)
{
    struct i2c conn;
    struct mpu9250 sensor;

    i2c_setup(&conn);
    mpu9250_setup(&sensor, &conn);
    mpu9250_ping(&sensor);

    return 0;
}

int test_mpu9250_set_get_gyro_scale(void)
{
    struct i2c conn;
    struct mpu9250 sensor;
    int8_t scale;

    /* setup */
    i2c_setup(&conn);
    mpu9250_setup(&sensor, &conn);

    scale = 2;
    mpu9250_set_gyro_scale(&sensor, scale);

    scale = 0;
    scale = mpu9250_get_gyro_scale(&sensor);
    mu_check(scale == 2);

    return 0;
}

int test_mpu9250_set_get_accel_scale(void)
{
    struct i2c conn;
    struct mpu9250 sensor;
    int8_t scale;

    /* setup */
    i2c_setup(&conn);
    mpu9250_setup(&sensor, &conn);

    scale = 2;
    mpu9250_set_accel_scale(&sensor, scale);

    scale = 0;
    scale = mpu9250_get_accel_scale(&sensor);
    mu_check(scale == 2);

    return 0;
}

int test_mpu9250_set_get_accel_fchoice(void)
{
    struct i2c conn;
    struct mpu9250 sensor;
    int8_t fchoice;

    /* setup */
    i2c_setup(&conn);
    mpu9250_setup(&sensor, &conn);

    fchoice = 1;
    mpu9250_set_accel_fchoice(&sensor, fchoice);

    fchoice = 0;
    fchoice = mpu9250_get_accel_fchoice(&sensor);
    mu_check(fchoice == 1);

    return 0;
}

void test_suite()
{
    mu_add_test(test_mpu9250_setup);
    mu_add_test(test_mpu9250_ping);
    mu_add_test(test_mpu9250_set_get_gyro_scale);
    mu_add_test(test_mpu9250_set_get_accel_scale);
    mu_add_test(test_mpu9250_set_get_accel_fchoice);
}

mu_run_tests(test_suite)
