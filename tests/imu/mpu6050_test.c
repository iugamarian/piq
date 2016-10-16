#include "munit.h"
#include "piq/imu/mpu6050.h"


#define TEST_OUTPUT "/tmp/test.output"


/* TESTS */
int test_mpu6050_setup(void);
int test_mpu6050_ping(void);
int test_mpu6050_data(void);
int test_mpu6050_calibrate(void);
int test_mpu6050_print(void);
int test_mpu6050_set_and_get_dplf_config(void);
void test_suite(void);


int test_mpu6050_setup(void)
{
    struct i2c conn;
    struct mpu6050 sensor;

    i2c_setup(&conn);
    mpu6050_setup(&sensor, &conn);

    return 0;
}

int test_mpu6050_ping(void)
{
    struct i2c conn;
    struct mpu6050 sensor;

    i2c_setup(&conn);
    mpu6050_setup(&sensor, &conn);
    mpu6050_ping(&sensor);

    return 0;
}

int test_mpu6050_data(void)
{
    struct i2c conn;
    struct mpu6050 sensor;

    i2c_setup(&conn);
    mpu6050_setup(&sensor, &conn);
    mpu6050_data(&sensor);
    mpu6050_data(&sensor);
    mpu6050_print(&sensor);

    mu_check(fltcmp(sensor.gyro.x, 0.0) != 0);
    mu_check(fltcmp(sensor.gyro.y, 0.0) != 0);
    mu_check(fltcmp(sensor.gyro.z, 0.0) != 0);

    mu_check(fltcmp(sensor.accel.x, 0.0) != 0);
    mu_check(fltcmp(sensor.accel.y, 0.0) != 0);
    mu_check(fltcmp(sensor.accel.z, 0.0) != 0);

    mu_check(fltcmp(sensor.accel.pitch, 0.0) != 0);
    mu_check(fltcmp(sensor.accel.roll, 0.0) != 0);

    mu_check(fltcmp(sensor.gyro.pitch, 0.0) != 0);
    mu_check(fltcmp(sensor.gyro.roll, 0.0) != 0);


    return 0;
}

int test_mpu6050_calibrate(void)
{
    struct i2c conn;
    struct mpu6050 sensor;

    i2c_setup(&conn);
    mpu6050_setup(&sensor, &conn);
    mpu6050_calibrate(&sensor);

    mu_check(fltcmp(sensor.accel.offset_x, 0.0) != 0);
    mu_check(fltcmp(sensor.accel.offset_y, 0.0) != 0);
    mu_check(fltcmp(sensor.accel.offset_z, 0.0) != 0);

    mu_check(fltcmp(sensor.gyro.offset_x, 0.0) != 0);
    mu_check(fltcmp(sensor.gyro.offset_y, 0.0) != 0);
    mu_check(fltcmp(sensor.gyro.offset_z, 0.0) != 0);

    return 0;
}

int test_mpu6050_print(void)
{
    struct i2c conn;
    struct mpu6050 sensor;

    i2c_setup(&conn);
    mpu6050_setup(&sensor, &conn);
    mpu6050_data(&sensor);
    mpu6050_print(&sensor);

    return 0;
}

int test_mpu6050_set_and_get_dplf_config(void)
{
    int8_t dplf_setting;
    struct i2c conn;
    struct mpu6050 sensor;

    i2c_setup(&conn);
    sensor.conn = &conn;

    mpu6050_set_dplf_config(&sensor, 5);
    dplf_setting = mpu6050_get_dplf_config(&sensor);
    mu_check(dplf_setting == 5);

    return 0;
}

int test_mpu6050_set_and_get_sample_rate_div(void)
{
    int8_t rate_div;
    struct i2c conn;
    struct mpu6050 sensor;

    i2c_setup(&conn);
    sensor.conn = &conn;

    mpu6050_set_sample_rate_div(&sensor, 1);
    rate_div = mpu6050_get_sample_rate_div(&sensor);
    mu_check(rate_div == 1);

    return 0;
}

int test_mpu6050_set_and_get_gyro_range(void)
{
    int8_t range;
    struct i2c conn;
    struct mpu6050 sensor;

    i2c_setup(&conn);
    sensor.conn = &conn;

    mpu6050_set_gyro_range(&sensor, 2);
    range = mpu6050_get_gyro_range(&sensor);
    mu_check(range == 2);

    return 0;
}

int test_mpu6050_set_and_get_accel_range(void)
{
    int8_t range;
    struct i2c conn;
    struct mpu6050 sensor;

    i2c_setup(&conn);
    sensor.conn = &conn;

    mpu6050_set_accel_range(&sensor, 2);
    range = mpu6050_get_accel_range(&sensor);
    mu_check(range == 2);

    return 0;
}

int test_mpu6050_info(void)
{
    struct i2c conn;
    struct mpu6050 sensor;

    i2c_setup(&conn);
    mpu6050_setup(&sensor, &conn);
    mpu6050_info(&sensor);

    return 0;
}

int test_mpu6050_record(void)
{
    struct i2c conn;
    struct mpu6050 sensor;

    FILE *output_file;
    char *line;
    size_t len;

    i2c_setup(&conn);
    mpu6050_setup(&sensor, &conn);
    mpu6050_record(&sensor, TEST_OUTPUT, 100);

    len = 0;
    output_file = fopen(TEST_OUTPUT, "r");
    while (getline(&line, &len, output_file) != -1) {
        printf("%s", line);
    }

    return 0;
}

void test_suite()
{
    mu_add_test(test_mpu6050_setup);
    mu_add_test(test_mpu6050_ping);
    mu_add_test(test_mpu6050_data);
    mu_add_test(test_mpu6050_calibrate);
    mu_add_test(test_mpu6050_print);
    mu_add_test(test_mpu6050_set_and_get_dplf_config);
    /* mu_add_test(test_mpu6050_set_and_get_sample_rate_div); */
    mu_add_test(test_mpu6050_set_and_get_gyro_range);
    mu_add_test(test_mpu6050_set_and_get_accel_range);
    mu_add_test(test_mpu6050_info);
    mu_add_test(test_mpu6050_record);
}

mu_run_tests(test_suite)
