#ifndef __I2C_HPP__
#define __I2C_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>

#include <linux/i2c-dev.h>

#include "piq/util.h"


// ERROR MESSAGES
#define I2C_INIT_FAILED "failed to initialize I2C!"

// DEFINES
#define I2C_BUF_MAX 1024


namespace piq {
namespace comm {

class I2C
{
public:
    int fd;

    I2C(void);
    ~I2C(void);
    int setup(void);
    int setSlave(char slave_addr);
    int readBytes(char reg_addr, char *data, size_t length);
    int readByte(char reg_addr, char *data);
    int writeByte(char reg_addr, char byte);
    int writeBytes(char reg_addr, char *data, size_t length);
};

}  // eof comm
}  // eof piq
#endif
