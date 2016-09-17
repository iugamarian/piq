# i2c.h

**Content:**
- Functions


## Functions

    int i2c_setup(void)
    void i2c_teardown(void)

Setup and teardown i2c connection. They must be called before and after all i2c
calls. Returns `0` on success, `-1` on failure.

---

    void i2c_set_slave(char slave_addr)

Sets the slave address for following i2c calls.

---

    int i2c_read_bytes(char reg_addr, char *data, size_t data_length)

Read bytes from register `reg_addr`, the data of length `data_length` will be
assigned to `data`. Returns `0` on success, `-1` on failure.

---

    int i2c_write_bytes(char reg_addr, char *data, size_t data_length)
    int i2c_write_byte(char reg_addr, uint8_t byte)

Write data of length `data_length` from `data` or `byte` data to register
`reg_addr`. Returns `0` on success, `-1` on failure.
