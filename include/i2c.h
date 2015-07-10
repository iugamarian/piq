#ifndef __I2C_H__
#define __I2C_H__

#include <stdio.h>
#include <stdlib.h>

#include <bcm2835.h>


/* ERROR MESSAGES */
#define I2C_INIT_FAILED "failed to initialize I2C!"


/* FUNCTIONS */
int i2c_setup(void);
void i2c_teardown(void);
void i2c_set_slave(char slave_addr);
int i2c_read_bytes(char reg_addr, char *data, size_t data_length);
int i2c_read_byte(char reg_addr, char *data);
int i2c_write_bytes(char reg_addr, char *data, size_t data_length);
int i2c_write_byte(char reg_addr, uint8_t byte);
int i2c_write_raw_byte(uint8_t byte);

#endif
