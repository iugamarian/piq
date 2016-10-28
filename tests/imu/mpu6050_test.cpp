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
int testMPU6050SetAndGetSampleRateDiv(void);
int testMPU6050SetAndGetGyroRange(void);
int testMPU6050SetAndGetAccelRange(void);
int testMPU6050Info(void);
int testMPU6050Record(void);
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

int testMPU6050SetAndGetSampleRateDiv(void)
{
    int8_t rate_div;
    piq::imu::MPU6050 sensor;

    sensor.configure();
    sensor.setSampleRateDiv(1);
    rate_div = sensor.getSampleRateDiv();
    mu_print("rate_div: %d\n", rate_div);
    mu_check(rate_div == 1);

    return 0;
}

int testMPU6050SetAndGetGyroRange(void)
{
    int8_t range;
    piq::imu::MPU6050 sensor;

    sensor.configure();
    sensor.setGyroRange(2);
    range = sensor.getGyroRange();
    mu_check(range == 2);

    return 0;
}

int testMPU6050SetAndGetAccelRange(void)
{
    int8_t range;
    piq::imu::MPU6050 sensor;

    sensor.configure();
    sensor.setGyroRange(2);
    sensor.setAccelRange(2);
    range = sensor.getAccelRange();
    mu_check(range == 2);

    return 0;
}

int testMPU6050Info(void)
{
    piq::imu::MPU6050 sensor;

    sensor.configure();
    sensor.info();

    return 0;
}

int testMPU6050Record(void)
{
    piq::imu::MPU6050 sensor;
    FILE *output_file;
    char *line;
    size_t len;

    sensor.configure();
    sensor.info();
    sensor.record(TEST_OUTPUT, 100);

    len = 0;
    output_file = fopen(TEST_OUTPUT, "r");
    while (getline(&line, &len, output_file) != -1) {
        printf("%s", line);
    }

    return 0;
}

void testSuite(void)
{
    mu_add_test(testMPU6050Configure);
    mu_add_test(testMPU6050Ping);
    mu_add_test(testMPU6050Data);
    mu_add_test(testMPU6050Calibrate);
    mu_add_test(testMPU6050Print);
    mu_add_test(testMPU6050SetAndGetDPLFConfig);
    mu_add_test(testMPU6050SetAndGetSampleRateDiv);
    mu_add_test(testMPU6050SetAndGetGyroRange);
    mu_add_test(testMPU6050SetAndGetAccelRange);
    mu_add_test(testMPU6050Info);
    mu_add_test(testMPU6050Record);
}

mu_run_tests(testSuite)
