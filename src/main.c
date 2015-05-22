#include "main.h"

#define SLAVE_ADDRESS 0x68

int mpu6050_setup(void)
{
    int retval;
    char buf[100];
    char PWR_MGMT_REG = 0x6B;
    char value = 0x00;

    /* configure i2c */
    retval = bcm2835_init();
    if (retval == 0) {
        printf("%s\n", I2C_INIT_FAILED);
        return -1;
    }
    bcm2835_i2c_setSlaveAddress(SLAVE_ADDRESS);

    /* POWER MANAGEMENT */
    bcm2835_i2c_begin();
    value = 0x00;
    buf[0] = MPU6050_RA_PWR_MGMT_1;
    buf[1] = 0x00;
    bcm2835_i2c_write(&buf, 2);

    return 0;
}

int main(void)
{
    /* int i; */
    char value[1];
    /* char bytes_high[1]; */
    /* char bytes_low[1]; */

    char WHO_AM_I = 0x75;
    char ACCEL_XOUT_H = 0x3B;
    /* char GYRO_XOUT_H = 0x43; */
    /* char GYRO_XOUT_L = 0x44; */

    bcm2835I2CReasonCodes retval;


    /* setup */
    mpu6050_setup();

    /* bcm2835_i2c_begin(); */
    /* bcm2835_i2c_read_register_rs(&WHO_AM_I, value, 1); */
    /* printf("who_am_i: %c\n", value[0]); */
    /* bcm2835_i2c_end(); */

    while (1) {
        char data[14];
        retval = bcm2835_i2c_write_read_rs(&ACCEL_XOUT_H, 1, data, 14);
        if (retval != BCM2835_I2C_REASON_OK) {
            printf("NOT OK!\n");
        }

        int i;
        /* for (i = 0; i < 14; i++) { */
            printf("i: %d\n", data[0]);
        /* } */
    }

    /* for (i = 0; i < 10; i++) { */
    /*     #<{(| start i2c |)}># */
    /*     bcm2835_i2c_begin(); */
    /*  */
    /*     bytes_high[0] = 0x00; */
    /*     retval = bcm2835_i2c_read_register_rs(&GYRO_XOUT_H, bytes_high, 1); */
    /*     if (retval != BCM2835_I2C_REASON_OK) { */
    /*         printf("NOT OK! X_H\n"); */
    /*     } */
    /*  */
    /*     bytes_low[0] = 0x00; */
    /*     retval = bcm2835_i2c_read_register_rs(&GYRO_XOUT_L, bytes_low, 1); */
    /*     if (retval != BCM2835_I2C_REASON_OK) { */
    /*         printf("NOT OK! X_L\n"); */
    /*     } */
    /*  */
    /*     #<{(| value |= bytes_low; |)}># */
    /*     #<{(| printf("gyro_x: %" PRIu16 "\n", value); |)}># */
    /*     printf("gyro_x_h: %d\n", bytes_high[0]); */
    /*     printf("gyro_x_l: %d\n", bytes_low[0]); */
    /*     printf("\n"); */
    /*  */
    /*     #<{(| end i2c |)}># */
    /*     bcm2835_i2c_end(); */
    /* } */

    bcm2835_i2c_end();

    return 0;
}
