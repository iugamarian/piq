#ifndef __I2C_H__
#define __I2C_H__

#include <stdio.h>

#include <bcm2835.h>


/* ERROR MESSAGES */
#define I2C_INIT_FAILED "failed to initialize I2C!"


/* FUNCTIONS */
int i2c_setup(void);
void i2c_teardown(void);


#endif
