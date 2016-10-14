#include "munit.h"
#include "piq/imu/imu.h"


/* PROTOTYPES */
int test_imu_setup(void);
void test_suite(void);


int test_imu_setup(void)
{
    struct imu imu;

    imu_setup(&imu);

    mu_check(fltcmp(imu.gyro.offset_x, 0.0f) == 0);
    mu_check(fltcmp(imu.gyro.offset_y, 0.0f) == 0);
    mu_check(fltcmp(imu.gyro.offset_z, 0.0f) == 0);
    mu_check(fltcmp(imu.gyro.pitch, 0.0f) == 0);
    mu_check(fltcmp(imu.gyro.roll, 0.0f) == 0);

    mu_check(fltcmp(imu.accel.offset_x, 0.0f) == 0);
    mu_check(fltcmp(imu.accel.offset_y, 0.0f) == 0);
    mu_check(fltcmp(imu.accel.offset_z, 0.0f) == 0);
    mu_check(fltcmp(imu.accel.pitch, 0.0f) == 0);
    mu_check(fltcmp(imu.accel.roll, 0.0f) == 0);

    mu_check(fltcmp(imu.pitch, 0.0f) == 0);
    mu_check(fltcmp(imu.roll, 0.0f) == 0);

    mu_check(fltcmp(imu.sample_rate, -1.0f) == 0);

    return 0;
}

void test_suite()
{
    mu_add_test(test_imu_setup);
}

mu_run_tests(test_suite)
