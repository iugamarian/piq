#include "piq.h"


int main(void)
{
    /* struct mpu6050_data data; */

    /* setup */
    mpu6050_setup();
    /* mpu6050_calibrate(); */
    mpu6050_gyroscope_test();
    mpu6050_accelerometer_test();

    /* read values */
    /* printf("\n"); */
    /* while (1) { */
    /*     mpu6050_data(&data); */
    /*     mpu6050_data_print(&data); */
    /*     sleep(2); */
    /* } */

    /* clean up */
    mpu6050_teardown();

    return 0;
}
