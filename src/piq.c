#include "piq.h"


void *mpu6050_loop(void *arg)
{
    struct piq *p;

    p = arg;
    while (p->state == PIQ_RUN) {
        mpu6050_data(p->imu);
    }

    return NULL;
}

struct piq *piq_setup(void)
{
    struct piq *p;

    p = malloc(sizeof(struct piq));
    p->state = PIQ_RUN;
    p->imu = mpu6050_setup();
    p->motors = esc_setup();

    return p;
}

void piq_destroy(void *target)
{
    struct piq *p;
    p = target;
    mpu6050_destroy(p->imu);
    esc_destroy(p->motors);
    free(p);
}

int main(void)
{
    struct piq *p;
    pthread_t thread_1;

    /* setup */
    log_info("setup");
    i2c_setup();
    p = piq_setup();

    pthread_create(&thread_1, NULL, &comms_loop, (void *) p);
    pthread_join(thread_1, NULL);

    /* teardown */
    piq_destroy(p);
    i2c_teardown();

    return 0;
}
