#include "munit.h"
#include "piq/comm/i2c.h"


/* TESTS */
int testI2CSetup(void);
int testI2CReadByte(void);
void testSuite(void);


int testI2CSetup(void)
{
    piq::comm::I2C i2c;

    i2c.setup();

    return 0;
}

int testI2CReadByte(void)
{
    char data;
    piq::comm::I2C i2c;

    i2c.setup();
    i2c.set_slave(0x68);
    i2c.read_byte(0x75, &data);

    printf("got: 0x%02X\n", (int) data);

    return 0;
}

void test_suite()
{
    mu_add_test(testI2CSetup);
    mu_add_test(testI2CReadByte);
}

mu_run_tests(test_suite)
