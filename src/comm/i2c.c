#include "comm/i2c.h"


int i2c_setup(struct i2c *conn)
{
    int fd;
    int adapter_nr;
    char filename[20];

    /* setup */
    adapter_nr = 1;  /* probably dynamically determined */
    memset(filename, '\0', sizeof(char) * 20);
    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);

    /* open i2c connection */
    fd = open(filename, O_RDWR);
    if (fd < 0) {
        return -1;
    } else {
        conn->fd = fd;
    }

    return 0;
}

int i2c_set_slave(struct i2c *conn, char slave_addr)
{
    return ioctl(conn->fd, I2C_SLAVE , slave_addr);
}

int i2c_read_byte(struct i2c *conn, char reg_addr, char *data)
{
    char buf[1];

    buf[0] = reg_addr;
    if (write(conn->fd, buf, 1) != 1) {
        return -1;
    }

    if (read(conn->fd, data, 1) != 1) {
        return -2;
    }

    return 0;
}

int i2c_read_bytes(struct i2c *conn, char reg_addr, char *data, size_t length)
{
    char buf[1];

    buf[0] = reg_addr;
    if (write(conn->fd, buf, 1) != 1) {
        return -1;
    }

    if (read(conn->fd, data, length) != 1) {
        return -2;
    }

    return 0;
}

int i2c_write_byte(struct i2c *conn, char reg_addr, char byte)
{
    char buf[2];

    buf[0] = reg_addr;
    buf[1] = byte;
    if (write(conn->fd, buf, 2) != 1) {
        return -1;
    }

    return 0;
}

int i2c_write_bytes(struct i2c *conn, char reg_addr, char *data, size_t length)
{
    int i;
    char buf[I2C_BUF_MAX];

    /* create buf */
    memset(buf, '\0', sizeof(char) * I2C_BUF_MAX);
    buf[0] = reg_addr;
    for (i = 1; i < length + 1; i++) {
        buf[i] = data[i];
    }

    /* write bytes */
    if (write(conn->fd, buf, length + 1) != 1) {
        return -1;
    }

    return 0;
}
