#include "utils.h"


static struct terminal_settings *init_term_io_settings(int echo)
{
    struct terminal_settings *ts;

    /* grab old i/o settings and make a copy */
    ts = malloc(sizeof(struct terminal_settings));
    tcgetattr(0, &ts->old);
    ts->new = ts->old;

    /* disable buffered i/o */
    ts->new.c_lflag &= ~ICANON;

    /* set echo mode */
    ts->new.c_lflag &= echo ? ECHO : ~ECHO;

    /* use new terminal i/o */
    tcsetattr(0, TCSANOW, &ts->new);

    return ts;
}

static void reset_term_io_settings(struct terminal_settings *ts)
{
    /* reset new settings with old */
    tcsetattr(0, TCSANOW, &ts->old);
}

static char getch_(int echo)
{
    char ch;
    struct terminal_settings *ts;

    ts = init_term_io_settings(echo);
    ch = getchar();
    reset_term_io_settings(ts);
    free(ts);

    return ch;
}

char getch(void)
{
    /* read 1 character without echo */
    return getch_(0);
}

char getche(void)
{
    /* read 1 character with echo */
    return getch_(1);
}
