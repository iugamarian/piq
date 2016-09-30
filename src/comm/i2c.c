#include "comm/i2c.h"


int i2c_setup(void)
{

    return 0;
}

void i2c_teardown(void)
{

}

void i2c_set_slave(char slave_addr)
{

}

int i2c_read_byte(char reg_addr, char *data)
{
    char buf[1];

    return 0;
}

int i2c_read_bytes(char reg_addr, char *data, size_t data_length)
{
    char buf[1];

    return 0;
}

int i2c_write_bytes(char reg_addr, char *data, size_t data_length)
{
    char *buf;
    int i;

    /* setup */
    buf = malloc(sizeof(char) * data_length + 1);

    /* create buf */
    buf[0] = reg_addr;
    for (i = 1; i < data_length + 1; i++) {
        buf[i] = data[i];
    }

    /* write bytes */

    /* clean up */
    free(buf);

    return 0;
}

int i2c_write_byte(char reg_addr, char byte)
{
    char data[2];

    /* setup */
    data[0] = reg_addr;
    data[1] = byte;

    /* write byte */

    return 0;
}

int i2c_write_raw_byte(uint8_t byte)
{
    char data[1];

    /* setup */
    data[0] = byte;

    /* write byte */

    return 0;
}
