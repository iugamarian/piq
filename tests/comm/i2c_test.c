#include "munit.h"
#include "comm/i2c.h"


/* TESTS */
int test_i2c_setup(void);
int test_i2c_read_byte(void);
void test_suite(void);


int test_i2c_setup(void)
{
    struct i2c conn;

    i2c_setup(&conn);

    return 0;
}

int test_i2c_read_byte(void)
{
    char data;
    struct i2c conn;

    i2c_setup(&conn);
    i2c_set_slave(&conn, 0x68);
    i2c_read_byte(&conn, 0x75, &data);
   
    printf("got: 0x%02X\n", (int) data);

    return 0;
}

void test_suite()
{
    mu_add_test(test_i2c_setup);
    mu_add_test(test_i2c_read_byte);
}

mu_run_tests(test_suite)
