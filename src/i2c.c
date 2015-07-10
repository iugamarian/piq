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

void i2c_set_slave(char slave_addr)
{
    bcm2835_i2c_setSlaveAddress(slave_addr);
}

int i2c_read_byte(char reg_addr, char *data)
{
    char buf[1];
    bcm2835I2CReasonCodes retval;

    /* setup */
    buf[0] = reg_addr;

    /* read bytes */
    retval = bcm2835_i2c_read_register_rs(buf, data, 1);
    if (retval != BCM2835_I2C_REASON_OK) {
        return -1;
    }

    return 0;
}

int i2c_read_bytes(char reg_addr, char *data, size_t data_length)
{
    char buf[1];
    bcm2835I2CReasonCodes retval;

    /* setup */
    buf[0] = reg_addr;

    /* read bytes */
    retval = bcm2835_i2c_write_read_rs(buf, 1, data, data_length);
    if (retval != BCM2835_I2C_REASON_OK) {
        return -1;
    }

    return 0;
}

int i2c_write_bytes(char reg_addr, char *data, size_t data_length)
{
    char *buf;
    int i;
    bcm2835I2CReasonCodes retval;

    /* setup */
    buf = malloc(sizeof(char) * data_length + 1);

    /* create buf */
    buf[0] = reg_addr;
    for (i = 1; i < data_length + 1; i++) {
        buf[i] = data[i];
    }

    /* write bytes */
    retval = bcm2835_i2c_write(buf, data_length + 1);
    if (retval != BCM2835_I2C_REASON_OK) {
        return -1;
    }

    /* clean up */
    free(buf);

    return 0;
}

int i2c_write_byte(char reg_addr, uint8_t byte)
{
    char data[2];
    bcm2835I2CReasonCodes retval;

    /* setup */
    data[0] = reg_addr;
    data[1] = byte;

    /* write byte */
    retval = bcm2835_i2c_write(data, 2);
    if (retval != BCM2835_I2C_REASON_OK) {
        return -1;
    }

    return 0;
}

int i2c_write_raw_byte(uint8_t byte)
{
    char data[1];
    bcm2835I2CReasonCodes retval;

    /* setup */
    data[0] = byte;

    /* write byte */
    retval = bcm2835_i2c_write(data, 1);
    if (retval != BCM2835_I2C_REASON_OK) {
        return -1;
    }

    return 0;
}
