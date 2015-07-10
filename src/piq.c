#include "piq.h"


static void manual_control_handler(char c, struct motors *m)
{
    /* keyboard handler */
    if (c == 'w') {
        log_info("increasing throttle");
        m->motor_1 += 100;
        m->motor_2 += 100;
        m->motor_3 += 100;
        m->motor_4 += 100;

    } else if (c == 's') {
        log_info("decreasing throttle");
        m->motor_1 -= 100;
        m->motor_2 -= 100;
        m->motor_3 -= 100;
        m->motor_4 -= 100;

    } else if (c == 'c') {
        log_info("calibrating motors");
        motors_calibrate(&m);

    } else if (c == 'r') {
        log_info("resetting PCA9685");
        pca9685_reset();

    }

    log_info("motor_1: %d", m->motor_1);
    log_info("motor_2: %d", m->motor_2);
    log_info("motor_3: %d", m->motor_3);
    log_info("motor_4: %d", m->motor_4);

    /* set throttle */
    motors_set_throttles(m);
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

    /* setup */
    log_info("setup");
    i2c_setup();
    m = motors_setup();

    /* events loop */
    while (1) {
        manual_control(&m);
    }

    /* clean up */
    i2c_teardown();

    return 0;
}
