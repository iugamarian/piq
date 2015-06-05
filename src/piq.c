#include "piq.h"


int main(void)
{
    struct mpu6050_data data;
    FILE *output_file;

    /* setup */
    i2c_setup();
    mpu6050_setup(&data);

    output_file = fopen("output.dat", "w");

    /* read values */
    log_info("running\n");
    int i = 0;
    while (1) {
        mpu6050_data(&data);
        mpu6050_record_data(output_file, &data);

        if (i == 10000) {
            break;
        }

        i++;
    }

    /* clean up */
    fclose(output_file);
    i2c_teardown();

    return 0;
}
