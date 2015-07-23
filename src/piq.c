#include "piq.h"


void *mpu6050_loop(void *arg)
{
    struct piq *p;

    p = arg;
    while (p->imu->state) {
        mpu6050_data(p->imu);
    }

    return NULL;
}

struct piq *piq_setup(void)
{
    struct piq *p;

    p = malloc(sizeof(struct piq));
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
    pthread_t thread_2;

    /* setup */
    log_info("setup");
    i2c_setup();
    p = piq_setup();

    /* thread telemetry and mpu6050 */
    pthread_create(&thread_1, NULL, &mpu6050_loop, (void *) p);
    pthread_create(&thread_2, NULL, &telemetry_loop, (void *) p);
    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    /* teardown */
    piq_destroy(p);
    i2c_teardown();

    return 0;
}
