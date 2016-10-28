#include "munit.hpp"
#include "piq/imu/mpu6050.hpp"


#define TEST_OUTPUT "/tmp/test.output"


/* TESTS */
int testMPU6050Configure(void);
int testMPU6050Ping(void);
int testMPU6050Data(void);
int testMPU6050Calibrate(void);
int testMPU6050Print(void);
int testMPU6050SetAndGetDPLFConfig(void);
void testSuite(void);


int testMPU6050Configure(void)
{
    piq::imu::MPU6050 sensor;

    sensor.configure();

    return 0;
}

int testMPU6050Ping(void)
{
    piq::imu::MPU6050 sensor;

    sensor.configure();
    sensor.ping();

    return 0;
}

int testMPU6050Data(void)
{
    piq::imu::MPU6050 sensor;

    sensor.configure();
    sensor.getData();
    sensor.getData();
    sensor.print();

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

int testMPU6050Calibrate(void)
{
    piq::imu::MPU6050 sensor;

    sensor.configure();
    sensor.calibrate();

    mu_check(fltcmp(sensor.accel.offset_x, 0.0) != 0);
    mu_check(fltcmp(sensor.accel.offset_y, 0.0) != 0);
    mu_check(fltcmp(sensor.accel.offset_z, 0.0) != 0);

    mu_check(fltcmp(sensor.gyro.offset_x, 0.0) != 0);
    mu_check(fltcmp(sensor.gyro.offset_y, 0.0) != 0);
    mu_check(fltcmp(sensor.gyro.offset_z, 0.0) != 0);

    return 0;
}

int testMPU6050Print(void)
{
    piq::imu::MPU6050 sensor;

    sensor.configure();
    sensor.getData();
    sensor.getData();
    sensor.print();

    return 0;
}

int testMPU6050SetAndGetDPLFConfig(void)
{
    int8_t dplf_setting;
    piq::imu::MPU6050 sensor;

    sensor.configure();
    sensor.setDPLFConfig(5);
    dplf_setting = sensor.getDPLFConfig();
    mu_check(dplf_setting == 5);

    return 0;
}

/* int testMPU6050set_and_get_sample_rate_div(void) */
/* { */
/*     int8_t rate_div; */
/*     struct i2c conn; */
/*     struct mpu6050 sensor; */
/*  */
/*     i2c_setup(&conn); */
/*     sensor.conn = &conn; */
/*  */
/*     mpu6050_set_sample_rate_div(&sensor, 1); */
/*     rate_div = mpu6050_get_sample_rate_div(&sensor); */
/*     mu_check(rate_div == 1); */
/*  */
/*     return 0; */
/* } */
/*  */
/* int testMPU6050set_and_get_gyro_range(void) */
/* { */
/*     int8_t range; */
/*     struct i2c conn; */
/*     struct mpu6050 sensor; */
/*  */
/*     i2c_setup(&conn); */
/*     sensor.conn = &conn; */
/*  */
/*     mpu6050_set_gyro_range(&sensor, 2); */
/*     range = mpu6050_get_gyro_range(&sensor); */
/*     mu_check(range == 2); */
/*  */
/*     return 0; */
/* } */
/*  */
/* int testMPU6050set_and_get_accel_range(void) */
/* { */
/*     int8_t range; */
/*     struct i2c conn; */
/*     struct mpu6050 sensor; */
/*  */
/*     i2c_setup(&conn); */
/*     sensor.conn = &conn; */
/*  */
/*     mpu6050_set_accel_range(&sensor, 2); */
/*     range = mpu6050_get_accel_range(&sensor); */
/*     mu_check(range == 2); */
/*  */
/*     return 0; */
/* } */
/*  */
/* int testMPU6050info(void) */
/* { */
/*     struct i2c conn; */
/*     struct mpu6050 sensor; */
/*  */
/*     i2c_setup(&conn); */
/*     mpu6050_setup(&sensor, &conn); */
/*     mpu6050_info(&sensor); */
/*  */
/*     return 0; */
/* } */
/*  */
/* int testMPU6050record(void) */
/* { */
/*     struct i2c conn; */
/*     struct mpu6050 sensor; */
/*  */
/*     FILE *output_file; */
/*     char *line; */
/*     size_t len; */
/*  */
/*     i2c_setup(&conn); */
/*     mpu6050_setup(&sensor, &conn); */
/*     mpu6050_record(&sensor, TEST_OUTPUT, 100); */
/*  */
/*     len = 0; */
/*     output_file = fopen(TEST_OUTPUT, "r"); */
/*     while (getline(&line, &len, output_file) != -1) { */
/*         printf("%s", line); */
/*     } */
/*  */
/*     return 0; */
/* } */

void testSuite(void)
{
    mu_add_test(testMPU6050Configure);
    mu_add_test(testMPU6050Ping);
    mu_add_test(testMPU6050Data);
    mu_add_test(testMPU6050Calibrate);
    mu_add_test(testMPU6050Print);
    mu_add_test(testMPU6050SetAndGetDPLFConfig);
    /* mu_add_test(testMPU6050set_and_get_sample_rate_div); */
    /* mu_add_test(testMPU6050set_and_get_gyro_range); */
    /* mu_add_test(testMPU6050set_and_get_accel_range); */
    /* mu_add_test(testMPU6050info); */
    /* mu_add_test(testMPU6050record); */
}

mu_run_tests(testSuite)
