#ifndef __UTILS_H__
#define __UTILS_H__

#define log_err(M, ...) \
    fprintf(stderr,\
        "[ERROR] (%s:%d) " M "\n",\
        __FILE__,\
        __LINE__,\
        ##__VA_ARGS__\
    )
#define log_warn(M, ...) fprintf(stderr, "[WARN] " M "\n", ##__VA_ARGS__)
#define log_info(M, ...) fprintf(stderr, "[INFO] " M "\n", ##__VA_ARGS__)

#endif
