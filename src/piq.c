#include "piq.h"


int main(void)
{
    struct mpu6050_data data;
    clock_t time_start;
    clock_t time_now;
    double dt;
    FILE *output_file;

    /* setup */
    i2c_setup();
    mpu6050_setup(&data);
    mpu6050_info(&data);
    mpu6050_data(&data);
    mpu6050_data_print(&data);

    output_file = fopen("output.dat", "w");

    /* read values */
    time_start = clock();
    time_now = clock();
    dt = ((double) time_now - time_start) / CLOCKS_PER_SEC;

    printf("\n");
    printf("recording!\n");
    while (dt < 20) {
        mpu6050_data(&data);
        mpu6050_record_data(output_file, &data);

        time_now = clock();
        dt = ((double) time_now - time_start) / CLOCKS_PER_SEC;

        if (dt == 10) {
            printf("10 seconds ...\n");
        }
    }
    printf("done!\n");

    /* clean up */
    fclose(output_file);
    i2c_teardown();

    return 0;
}
