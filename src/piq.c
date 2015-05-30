#include "piq.h"


int main(void)
{
    /* struct mpu6050_data data; */

    /* setup */
    mpu6050_setup();
    /* mpu6050_calibrate(); */

    /* read values */
    mpu6050_set_sample_rate_div(0);
    mpu6050_set_gyro_range(0);
    printf("sample_rate_div: %d\n", mpu6050_get_sample_rate_div());
    printf("gyro_range: %d\n", mpu6050_get_gyro_range());
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
