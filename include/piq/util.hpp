#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/time.h>


/* MACROS */
#ifdef NDEBUG
    #define debug(M, ...)
#else
    #define debug(M, ...) \
        fprintf( \
            stderr, \
            "[DEBUG] %s:%d: " M "\n", \
            __func__, \
            __LINE__, \
            ##__VA_ARGS__ \
        )
#endif


#define log_err(M, ...) \
    fprintf(stderr,\
        "[ERROR] (%s:%d) " M "\n",\
        __FILE__,\
        __LINE__,\
        ##__VA_ARGS__\
    )
#define log_warn(M, ...) fprintf(stderr, "[WARN] " M "\n", ##__VA_ARGS__)
#define log_info(M, ...) fprintf(stderr, "[INFO] " M "\n", ##__VA_ARGS__)

#define check(A, M, ...) \
    if (!(A)) { \
        log_err(M, ##__VA_ARGS__); \
        goto error; \
    }
#define check_mem(A) check((A), "out of memory.")
#define check_debug(A, M, ...) \
    if (!(A)) { \
        debug(M, ##__VA_ARGS__); \
        goto error; \
    }
#define silent_check(A) \
    if (!(A)) { \
        goto error; \
    }

#define free_mem(TARGET, FREE_FUNC) \
    if (TARGET) { \
        FREE_FUNC((void *) TARGET); \
    }


/* DEFINES */
#define TERMINAL_RAW_MODE 0
#define TERMINAL_COOKED_MODE 1


/* STRUCTURES */
struct terminal_settings
{
    int state;
    struct termios old_settings;
};


/* FUNCTIONS */
double deg2rad(double d);
double rad2deg(double r);
struct terminal_settings terminal_settings_new(void);
void terminal_cooked_mode(struct terminal_settings *ts);
void terminal_raw_mode(struct terminal_settings *ts);
void terminal_restore(struct terminal_settings *ts);
int terminal_kbhit(struct terminal_settings *ts);
int getch(void);
int getche(void);
int fltcmp(float v1, float v2);

#endif
