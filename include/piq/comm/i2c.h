#ifndef __I2C_H__
#define __I2C_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>

#include <linux/i2c-dev.h>

#include "piq/util.h"


/* ERROR MESSAGES */
#define I2C_INIT_FAILED "failed to initialize I2C!"

/* DEFINES */
#define I2C_BUF_MAX 1024


/* STRUCTURES */
struct i2c
{
    int fd;


};


/* FUNCTIONS */
int i2c_setup(struct i2c *conn);
int i2c_set_slave(struct i2c *conn, char slave_addr);
int i2c_read_bytes(struct i2c *conn, char reg_addr, char *data, size_t length);
int i2c_read_byte(struct i2c *conn, char reg_addr, char *data);
int i2c_write_byte(struct i2c *conn, char reg_addr, char byte);
int i2c_write_bytes(struct i2c *conn, char reg_addr, char *data, size_t length);

#endif
