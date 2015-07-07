#include "piq.h"


static struct termios g_old_kbd_mode;

static void cooked(void)
{
    tcsetattr(0, TCSANOW, &g_old_kbd_mode);
}

static void raw(void)
{
    static char init;
    struct termios new_kbd_mode;

    if (init) {
        return;
    }

    /* put keyboard (stdin, actually) in raw, unbuffered mode */
    tcgetattr(0, &g_old_kbd_mode);
    memcpy(&new_kbd_mode, &g_old_kbd_mode, sizeof(struct termios));
    new_kbd_mode.c_lflag &= ~(ICANON | ECHO);
    new_kbd_mode.c_cc[VTIME] = 0;
    new_kbd_mode.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_kbd_mode);

    /* when we exit, go back to normal, "cooked" mode */
    atexit(cooked);

    init = 1;
}

static int kbhit(void)
{
    struct timeval timeout;
    fd_set read_handles;
    int status;

    raw();

    /* check stdin (fd 0) for activity */
    FD_ZERO(&read_handles);
    FD_SET(0, &read_handles);
    timeout.tv_sec = timeout.tv_usec = 0;
    status = select(0 + 1, &read_handles, NULL, NULL, &timeout);

    if(status < 0)
    {
        printf("select() failed in kbhit()\n");
        exit(1);
    }

    return status;
}

int manual_control(void)
{
    int event;
    char c;

    while (1) {
        usleep(1);
        event = kbhit();

        if (event == 1) {
            c = getch();
            printf("--> %c\n", c);

            event = 0;
        } else {
            printf(".");
        }
    }


    return 0;
}


int main(void)
{

    manual_control();
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
