#include "i2c.h"


int i2c_setup(void)
{
    bcm2835I2CReasonCodes retval;

    retval = bcm2835_init();
    if (retval == 0) {
        printf("%s\n", I2C_INIT_FAILED);
        return -1;
    }
    bcm2835_i2c_begin();

    return 0;
}

void i2c_teardown(void)
{
    bcm2835_i2c_end();
}
