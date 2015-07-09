#include "piq.h"


static void manual_control_handler(char c, struct motors *m)
{
    if (c == 'w') {
        m->motor_1 += 100;
        m->motor_2 += 100;
        m->motor_3 += 100;
        m->motor_4 += 100;

    } else if (c == 's') {
        m->motor_1 -= 100;
        m->motor_2 -= 100;
        m->motor_3 -= 100;
        m->motor_4 -= 100;

    }

    log_info("motor_1: %d", m->motor_1);
    log_info("motor_2: %d", m->motor_2);
    log_info("motor_3: %d", m->motor_3);
    log_info("motor_4: %d", m->motor_4);
}

static int manual_control(struct motors *m)
{
    char c;
    struct terminal_settings ts;

    /* setup */
    ts = terminal_settings_new();

    /* obtain keyboard event */
    usleep(1);
    if (terminal_kbhit(&ts)) {
        c = getch();
        manual_control_handler(c, m);
    }

    /* clean up */
    terminal_restore(&ts);

    return 0;
}

int main(void)
{
    struct motors m;

    log_info("setup");

    i2c_setup();
    m = motors_setup();
    motors_set_throttles(&m);

    while (1) {
        manual_control(&m);
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
    /*  */
    return 0;
}
