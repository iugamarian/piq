#include "mpu6050.h"


int mpu6050_setup(void)
{
    char buf[2];

    /* setup */
    i2c_setup();
    bcm2835_i2c_setSlaveAddress(MPU6050_ADDRESS);

    /* POWER MANAGEMENT */
    buf[0] = MPU6050_RA_PWR_MGMT_1;
    buf[1] = 0x00;
    bcm2835_i2c_write(buf, 2);

    return 0;
}

void mpu6050_teardown(void)
{
    i2c_teardown();
}

int mpu6050_ping(void)
{
    char buf[1];
    char value[1];

    /* setup */
    buf[0] = MPU6050_RA_WHO_AM_I;
    value[0] = 0x00;

    /* print mpu6050 address */
    bcm2835_i2c_read_register_rs(buf, value, 1);
    printf("MPU6050 ADDRESS: 0x%02X\n", value[0]);

    return 0;
}

int mpu6050_data(struct mpu6050_data *data)
{
    char buf[1];
    char raw_data[14];
    bcm2835I2CReasonCodes retval;

    /* setup */
    buf[0] = MPU6050_RA_ACCEL_XOUT_H;
    memset(raw_data, 0, 14);

    /* read sensor data */
    retval = bcm2835_i2c_write_read_rs(buf, 1, raw_data, 14);
    if (retval != BCM2835_I2C_REASON_OK) {
        return -1;
    }

    /* accelerometer */
    data->accel.x = (raw_data[0] << 8) | (raw_data[1]);
    data->accel.y = (raw_data[2] << 8) | (raw_data[3]);
    data->accel.z = (raw_data[4] << 8) | (raw_data[5]);

    /* calculate roll */
    /* roll = arctan(accel_x / sqrt(pow(accel_y, 2) + pow(accel_z, 2))) */

    /* calculate pitch */
    /* pitch = arctan(accel_y / sqrt(pow(accel_x, 2) + pow(accel_z, 2))) */

    /* temperature */
    data->temperature = (raw_data[6] << 8) | (raw_data[7]);
    data->temperature = (data->temperature / 340) + 36.53;

    /* gyroscope */
    data->gyro.x = (raw_data[8] << 8) | (raw_data[9]);
    data->gyro.y = (raw_data[10] << 8) | (raw_data[11]);
    data->gyro.z = (raw_data[12] << 8) | (raw_data[13]);

    return 0;
}

int mpu6050_gyroscope_test(void)
{
    char w_data[2];
    char fs_sel;
    struct mpu6050_data data_before;
    struct mpu6050_data data_after;
    int16_t gyro_x_diff;
    int16_t gyro_y_diff;
    int16_t gyro_z_diff;

    /*
    This function performs a gyroscope self-test on the MPU6050 sensor, this is
    intiated by setting the `XG_ST`, `YG_ST` and `ZG_ST` to 1 respectively in
    the `GYRO_CONFIG` register.

    In the [Register Map document (page 14)][1] it says:

    > When the value of the self-test response is within the min/max limits >
    of the product specification, the part has passed self test.

    According to the datasheet (page 12), it states that the min/max is a
    relative +/- 14% of the sensor data, before and after the self-test is
    enabled.
    */

    /* gyro data before self-test enabled */
    mpu6050_data(&data_before);
    mpu6050_data_print(&data_before);

    /* enable gyro self test */
    fs_sel = 0x00;
    w_data[0] = MPU6050_RA_GYRO_CONFIG;
    w_data[1] = (
       (1 << 7)         /* XG_ST */
       | (1 << 6)       /* YG_ST */
       | (1 << 5)       /* ZG_ST */
       | (fs_sel << 3)  /* FS_SEL */
    );
    bcm2835_i2c_write(w_data, 2);

    /* gyro data after self-test enabled */
    mpu6050_data(&data_after);
    mpu6050_data_print(&data_after);

    /* disable gyro self test */
    fs_sel = 0x00;
    w_data[0] = MPU6050_RA_GYRO_CONFIG;
    w_data[1] = (fs_sel << 3);  /* FS_SEL */
    bcm2835_i2c_write(w_data, 2);

    /* calculate x, y, z before and after self-test enabled */
    gyro_x_diff = abs(data_after.gyro.x) - abs(data_before.gyro.x);
    gyro_y_diff = abs(data_after.gyro.y) - abs(data_before.gyro.y);
    gyro_z_diff = abs(data_after.gyro.z) - abs(data_before.gyro.z);

    printf("x: %d\n", gyro_x_diff);
    printf("y: %d\n", gyro_y_diff);
    printf("z: %d\n", gyro_z_diff);

    return 0;
}

int mpu6050_accelerometer_test(void)
{
    char w_data[2];
    char afs_sel;
    struct mpu6050_data data_before;
    struct mpu6050_data data_after;
    int16_t accel_x_diff;
    int16_t accel_y_diff;
    int16_t accel_z_diff;

    /*
    This function performs a accelerometer self-test on the MPU6050 sensor,
    this is intiated by setting the `XA_ST`, `YA_ST` and `ZA_ST` to 1
    respectively in the `ACCEL_CONFIG` register.

    In the [Register Map document (page 15)][1] it says:

    > When the value of the self-test response is within the min/max limits >
    of the product specification, the part has passed self test.

    According to the datasheet (page 13), it states that the min/max is a
    relative +/- 14% of the sensor data, before and after the self-test is
    enabled.
    */

    /* accel data before self-test enabled */
    mpu6050_data(&data_before);
    mpu6050_data_print(&data_before);

    /* enable accel self test */
    afs_sel = 0x00;
    w_data[0] = MPU6050_RA_ACCEL_CONFIG;
    w_data[1] = (
       (1 << 7)             /* XA_ST */
       | (1 << 6)           /* YA_ST */
       | (1 << 5)           /* ZA_ST */
       | (afs_sel << 3)     /* AFS_SEL */
    );
    bcm2835_i2c_write(w_data, 2);

    /* accel data after self-test enabled */
    mpu6050_data(&data_after);
    mpu6050_data_print(&data_after);

    /* disable accel self test */
    afs_sel = 0x00;
    w_data[0] = MPU6050_RA_GYRO_CONFIG;
    w_data[1] = (afs_sel << 3);  /* AFS_SEL */
    bcm2835_i2c_write(w_data, 2);

    /* calculate x, y, z before and after self-test enabled */
    accel_x_diff = abs(data_after.accel.x) - abs(data_before.accel.x);
    accel_y_diff = abs(data_after.accel.y) - abs(data_before.accel.y);
    accel_z_diff = abs(data_after.accel.z) - abs(data_before.accel.z);

    printf("x: %d\n", accel_x_diff);
    printf("y: %d\n", accel_y_diff);
    printf("z: %d\n", accel_z_diff);

    return 0;
}

int mpu6050_calibrate(void)
{
    int16_t i;
    int16_t offset_x;
    int16_t offset_y;
    int16_t offset_z;
    struct mpu6050_data data;

    for (i = 0; i < 5000; i++) {
        mpu6050_data(&data);

        offset_x += data.gyro.x;
        offset_y += data.gyro.y;
        offset_z += data.gyro.z;
    }

    offset_x = offset_x / 5000;
    offset_y = offset_y / 5000;
    offset_z = offset_z / 5000;

    printf("calibrate mpu6050\n");
    printf("offset_x: %d\n", offset_x);
    printf("offset_y: %d\n", offset_y);
    printf("offset_z: %d\n", offset_z);

    return 0;
}

void mpu6050_data_print(struct mpu6050_data *data)
{
    printf("gyro_x: %d\n", data->gyro.x);
    printf("gyro_y: %d\n", data->gyro.y);
    printf("gyro_z: %d\n", data->gyro.z);

    printf("accel x: %d\n", data->accel.x);
    printf("accel y: %d\n", data->accel.y);
    printf("accel z: %d\n", data->accel.z);

    printf("temp: %f\n", data->temperature);
    printf("\n");
}
