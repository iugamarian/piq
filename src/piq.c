#include "piq.h"


int manual_control(void)
{
    char c;
    struct terminal_settings ts;

    /* setup */
    ts = terminal_settings_new();

    /* obtain keyboard event */
    usleep(1);
    if (terminal_kbhit(&ts)) {
        c = getch();
        printf("--> %c\n", c);
    } else {
        printf(".");
    }

    /* clean up */
    terminal_restore(&ts);

    return 0;
}


int main(void)
{
    while (1) {
        manual_control();
    }

    /* struct mpu6050_data data; */
    /* FILE *output_file; */
    /* int8_t retval; */
    /*  */
    /* #<{(| setup |)}># */
    /* i2c_setup(); */
    /* mpu6050_setup(&data); */
    /*  */
    /* output_file = fopen("output.dat", "w"); */
    /*  */
    /* #<{(| read values |)}># */
    /* log_info("running\n"); */
    /* int i = 0; */
    /* while (1) { */
    /*     #<{(| get data |)}># */
    /*     retval = mpu6050_data(&data); */
    /*     if (retval == -1) { */
    /*         log_err("Failed to obtain data from MPU6050!"); */
    /*         return -1; */
    /*     } */
    /*  */
    /*     #<{(| record data |)}># */
    /*     mpu6050_record_data(output_file, &data); */
    /*     if (retval == -1) { */
    /*         log_err("Failed to record MPU6050 data!"); */
    /*         return -1; */
    /*     } */
    /*  */
    /*     if (i == 10000) { */
    /*         break; */
    /*     } */
    /*  */
    /*     i++; */
    /* } */
    /*  */
    /* #<{(| clean up |)}># */
    /* fclose(output_file); */
    /* i2c_teardown(); */

    return 0;
}
