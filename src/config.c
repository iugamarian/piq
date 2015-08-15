#include "config.h"


struct config *config_new(void)
{
    struct config *c;

    c = malloc(sizeof(struct config));

    /* mcc config */
    c->mcc_host = NULL;
    c->mcc_port = -1;

    /* pitch pid config */
    c->pitch.sample_rate = 100;
    c->pitch.k_p = 0.0f;
    c->pitch.k_i = 0.0f;
    c->pitch.k_d = 0.0f;
    c->pitch.min = -1.0f;
    c->pitch.max = 1.0f;

    /* roll pid config */
    c->roll.sample_rate = 100;
    c->roll.k_p = 0.0f;
    c->roll.k_i = 0.0f;
    c->roll.k_d = 0.0f;
    c->roll.min = -1.0f;
    c->roll.max = 1.0f;

    /* pitch & roll offset */
    c->pitch_offset = 0.0f;
    c->roll_offset = 0.0f;

    return c;
}

void config_destroy(void *target)
{
    struct config *c;
    c = target;
    free(c->mcc_host);
    free(c);
}

struct config *config_setup(void)
{
    struct config *c;

    c = config_new();
    config_load(c, "piq.config");

    return c;
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

        /* MCC CONFIG */
        if (strcmp(key, "mcc.host") == 0) {
            c->mcc_host = malloc(sizeof(char) * (strlen(val) + 1));
            strcpy(c->mcc_host, val);
        } else if (strcmp(key, "mcc.port") == 0) {
            c->mcc_port = atoi(val);

        /* PITCH PID */
        } else if (strcmp(key, "pitch.sample_rate") == 0) {
            c->pitch.sample_rate = atoi(val);
        } else if (strcmp(key, "pitch.k_p") == 0) {
            c->pitch.k_p = atof(val);
        } else if (strcmp(key, "pitch.k_i") == 0) {
            c->pitch.k_i = atof(val);
        } else if (strcmp(key, "pitch.k_d") == 0) {
            c->pitch.k_d = atof(val);
        } else if (strcmp(key, "pitch.min") == 0) {
            c->pitch.min = atof(val);
        } else if (strcmp(key, "pitch.max") == 0) {
            c->pitch.max = atof(val);

        /* ROLL PID */
        } else if (strcmp(key, "roll.sample_rate") == 0) {
            c->roll.sample_rate = atoi(val);
        } else if (strcmp(key, "roll.k_p") == 0) {
            c->roll.k_p = atof(val);
        } else if (strcmp(key, "roll.k_i") == 0) {
            c->roll.k_i = atof(val);
        } else if (strcmp(key, "roll.k_d") == 0) {
            c->roll.k_d = atof(val);
        } else if (strcmp(key, "roll.min") == 0) {
            c->roll.min = atof(val);
        } else if (strcmp(key, "roll.max") == 0) {
            c->roll.max = atof(val);

        /* PITCH & ROLL OFFSETS */
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
    printf("mcc config:\n");
    printf("--------------------\n");
    printf("mcc.host: %s\n", c->mcc_host);
    printf("mcc.port: %d\n", c->mcc_port);

    printf("\n");
    printf("pitch pid config:\n");
    printf("--------------------\n");
    printf("pitch.sample_rate : %d\n", c->pitch.sample_rate);
    printf("pitch.k_p: %f\n", c->pitch.k_p);
    printf("pitch.k_i: %f\n", c->pitch.k_i);
    printf("pitch.k_d: %f\n", c->pitch.k_d);
    printf("pitch.min: %f\n", c->pitch.min);
    printf("pitch.max: %f\n", c->pitch.max);

    printf("\n");
    printf("roll pid config:\n");
    printf("--------------------\n");
    printf("roll.sample_rate : %d\n", c->roll.sample_rate);
    printf("roll.k_p: %f\n", c->roll.k_p);
    printf("roll.k_i: %f\n", c->roll.k_i);
    printf("roll.k_d: %f\n", c->roll.k_d);
    printf("roll.min: %f\n", c->roll.min);
    printf("roll.max: %f\n", c->roll.max);

    printf("\n");
    printf("pitch and roll offsets:\n");
    printf("--------------------\n");
    printf("pitch.offset: %.2f\n", c->pitch_offset);
    printf("roll.offset: %.2f\n", c->roll_offset);
}
