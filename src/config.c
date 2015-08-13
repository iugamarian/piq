#include "config.h"


struct config *config_new(void)
{
    struct config *c;

    c = malloc(sizeof(struct config));

    c->pitch_k_p = 0.0f;
    c->pitch_k_i = 0.0f;
    c->pitch_k_d = 0.0f;

    c->roll_k_p = 0.0f;
    c->roll_k_i = 0.0f;
    c->roll_k_d = 0.0f;

    c->pitch_offset = 0.0f;
    c->roll_offset = 0.0f;

    return c;
}

void config_destroy(void *target)
{
    struct config *c;
    c = target;
    free(c);
}

int config_load(struct config *c, const char *file_path)
{
    char buf[50];
    char key[50];
    char val[50];
    FILE *config_fp;

    /* setup */
    config_fp = fopen(file_path, "r");
    check(config_fp != NULL, "failed to open file: %s", file_path);

    /* load configuration */
    memset(buf, '\0', 50);
    while (fgets(buf, sizeof(buf), config_fp)) {
        memset(key, '\0', sizeof(key));
        memset(val, '\0', sizeof(val));
        sscanf(buf, "%s %s", key, val);

        if (strcmp(key, "pitch.k_p") == 0) {
            c->pitch_k_p = atof(val);
        } else if (strcmp(key, "pitch.k_i") == 0) {
            c->pitch_k_i = atof(val);
        } else if (strcmp(key, "pitch.k_d") == 0) {
            c->pitch_k_d = atof(val);
        } else if (strcmp(key, "roll.k_p") == 0) {
            c->roll_k_p = atof(val);
        } else if (strcmp(key, "roll.k_i") == 0) {
            c->roll_k_i = atof(val);
        } else if (strcmp(key, "roll.k_d") == 0) {
            c->roll_k_d = atof(val);
        } else if (strcmp(key, "pitch.offset") == 0) {
            c->pitch_offset = atof(val);
        } else if (strcmp(key, "roll.offset") == 0) {
            c->roll_offset = atof(val);
        }
    }

    /* clean up */
    fclose(config_fp);

    return 0;
error:
    return -1;
}


void config_info(struct config *c)
{
    printf("pitch.k_p: %f\n", c->pitch_k_p);
    printf("pitch.k_i: %f\n", c->pitch_k_i);
    printf("pitch.k_d: %f\n", c->pitch_k_d);

    printf("roll.k_p: %f\n", c->roll_k_p);
    printf("roll.k_i: %f\n", c->roll_k_i);
    printf("roll.k_d: %f\n", c->roll_k_d);

    printf("pitch.offset: %f\n", c->pitch_offset);
    printf("roll.offset: %f\n", c->roll_offset);
}
