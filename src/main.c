#include "main.h"

#define SLAVE_ADDRESS 0x68


int main(void)
{
    int retval;
    char buf[2];

    /* setup */
    retval = bcm2835_init();
    if (retval) {
        printf("%s\n", I2C_INIT_FAILED);
        return -1;
    }
    bcm2835_i2c_setSlaveAddress(SLAVE_ADDRESS);
    bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_626);

    /* start i2c */
    bcm2835_i2c_begin();
    bcm2835_i2c_read(buf, 2);
    bcm2835_i2c_end();

    return 0;
}
