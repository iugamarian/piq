#include "piq/util.h"


struct terminal_settings terminal_settings_new(void)
{
    struct terminal_settings ts;
    ts.state = TERMINAL_COOKED_MODE;
    return ts;
}

void terminal_cooked_mode(struct terminal_settings *ts)
{
    if (ts->state == TERMINAL_RAW_MODE) {
        tcsetattr(0, TCSANOW, &ts->old_settings);
    }
}

void terminal_raw_mode(struct terminal_settings *ts)
{
    struct termios new_settings;

    if (ts->state == TERMINAL_COOKED_MODE) {
        /* put keyboard (stdin, actually) in raw, unbuffered mode */
        tcgetattr(0, &ts->old_settings);
        memcpy(&new_settings, &ts->old_settings, sizeof(struct termios));
        new_settings.c_lflag &= ~(ICANON | ECHO);
        new_settings.c_cc[VTIME] = 0;
        new_settings.c_cc[VMIN] = 1;
        tcsetattr(0, TCSANOW, &new_settings);
    }
}

void terminal_restore(struct terminal_settings *ts)
{
    terminal_cooked_mode(ts);
}

int terminal_kbhit(struct terminal_settings *ts)
{
    struct timeval timeout;
    fd_set read_handles;
    int status;

    /* setup */
    terminal_raw_mode(ts);

    /* check stdin (fd 0) for activity */
    FD_ZERO(&read_handles);
    FD_SET(0, &read_handles);
    timeout.tv_sec = timeout.tv_usec = 0;
    status = select(0 + 1, &read_handles, NULL, NULL, &timeout);

    if (status < 0) {
        printf("select() failed in kbhit()\n");
        exit(1);
    }

    return status;
}

int getch(void)
{
    int ch;
    struct termios oldattr;
    struct termios newattr;

    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);

    return ch;
}

int getche(void)
{
    int ch;
    struct termios oldattr;
    struct termios newattr;

    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON);

    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);

    return ch;
}

int fltcmp(float v1, float v2)
{
    /* compare floats */
    if (fabs(v1 - v2) <= 0.00001) {
        return 0;
    } else if (v1 > v2) {
        return 1;
    } else if (v1 < v2) {
        return -1;
    } else {
        log_err("Undefined runtime behaviour!");
        return -1;
    }
}
