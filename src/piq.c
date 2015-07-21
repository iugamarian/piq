#include "piq.h"


/* static int manual_control_handler(char c, struct esc *e) */
/* { */
/*     #<{(| keyboard handler |)}># */
/*     if (c == 'w') { */
/*         log_info("increasing throttle"); */
/*         e->motor_1 += 100; */
/*         e->motor_2 += 100; */
/*         e->motor_3 += 100; */
/*         e->motor_4 += 100; */
/*         esc_set_throttles(e); */
/*  */
/*         log_info("motor_1: %d", e->motor_1); */
/*         log_info("motor_2: %d", e->motor_2); */
/*         log_info("motor_3: %d", e->motor_3); */
/*         log_info("motor_4: %d", e->motor_4); */
/*  */
/*     } else if (c == 's') { */
/*         log_info("decreasing throttle"); */
/*         e->motor_1 -= 100; */
/*         e->motor_2 -= 100; */
/*         e->motor_3 -= 100; */
/*         e->motor_4 -= 100; */
/*         esc_set_throttles(e); */
/*  */
/*         log_info("motor_1: %d", e->motor_1); */
/*         log_info("motor_2: %d", e->motor_2); */
/*         log_info("motor_3: %d", e->motor_3); */
/*         log_info("motor_4: %d", e->motor_4); */
/*  */
/*     } else if (c == 'c') { */
/*         log_info("calibrating esc"); */
/*         esc_calibrate(e); */
/*  */
/*     } else if (c == 'r') { */
/*         log_info("resetting PCA9685"); */
/*         pca9685_reset(); */
/*  */
/*     } else if (c == 'q') { */
/*         log_info("stopping..."); */
/*         pca9685_reset(); */
/*         return 1; */
/*  */
/*     } */
/*  */
/*     return 0; */
/* } */
/*  */
/* static int manual_control(struct esc *m) */
/* { */
/*     char c; */
/*     int quit; */
/*     struct terminal_settings ts; */
/*  */
/*     #<{(| setup |)}># */
/*     ts = terminal_settings_new(); */
/*  */
/*     #<{(| obtain keyboard event |)}># */
/*     usleep(1); */
/*     quit = 0; */
/*     if (terminal_kbhit(&ts)) { */
/*         c = getch(); */
/*         quit = manual_control_handler(c, m); */
/*     } */
/*  */
/*     #<{(| clean up |)}># */
/*     terminal_restore(&ts); */
/*  */
/*     return quit; */
/* } */

static void *test_loop(void *arg)
{
    struct mpu6050_data *data;

    data = arg;
    while (data->state) {
        mpu6050_data(data);
    }

    return NULL;
}


int main(void)
{
    struct mpu6050_data *data;
    pthread_t thread_1;
    pthread_t thread_2;

    /* setup */
    log_info("setup");
    i2c_setup();
    data = mpu6050_new();
    mpu6050_setup(data);

    /* threading */
    pthread_create(&thread_1, NULL, &telemetry_loop, (void *) data);
    pthread_create(&thread_2, NULL, &test_loop, (void *) data);
    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    /* teardown */
    mpu6050_destroy(data);
    i2c_teardown();

    return 0;
}
