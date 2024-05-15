#ifndef DS4422_DS4424_H
#define DS4422_DS4424_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>

    typedef enum ds4422_ds4424_error_t
    {
        DS4422_DS4424_SUCCESS,
        DS4422_DS4424_ERROR_NULL_POINTER,
        DS4422_DS4424_ERROR_INVALID_SLAVE_ADDRESS,
        DS4422_DS4424_ERROR_INVALID_CURRENT_SOURCE,
        DS4422_DS4424_ERROR_INVALID_SIGN_BIT,
        DS4422_DS4424_ERROR_INVALID_DATA,
        DS4422_DS4424_ERROR_I2C_FAILURE
    } ds4422_ds4424_error_t;

    typedef enum ds4422_ds4424_i2c_slave_address_t
    {
        DS4422_DS4424_A0_GND_A1_GND = 0x20, /* 0b0010000x */
        DS4422_DS4424_A0_VCC_A1_GND = 0x60, /* 0b0110000x */
        DS4422_DS4424_A0_GND_A1_VCC = 0xA0, /* 0b1010000x */
        DS4422_DS4424_A0_VCC_A1_VCC = 0xE0  /* 0b1110000x */
    } ds4422_ds4424_i2c_slave_address_t;

    typedef struct ds4422_ds4424_i2c_master_t
    {
        ds4422_ds4424_error_t (*write)(struct ds4422_ds4424_i2c_master_t *, ds4422_ds4424_i2c_slave_address_t, uint8_t[], size_t);
    } ds4422_ds4424_i2c_master_t;

    ds4422_ds4424_error_t ds4422_ds4424_i2c_master_write(ds4422_ds4424_i2c_master_t *, ds4422_ds4424_i2c_slave_address_t, uint8_t[], size_t);

    typedef struct ds4422_ds4424_t
    {
        ds4422_ds4424_i2c_master_t *i2c_master;
        ds4422_ds4424_i2c_slave_address_t slave_address;
    } ds4422_ds4424_t;

    ds4422_ds4424_error_t ds4422_ds4424_init(ds4422_ds4424_t *, ds4422_ds4424_i2c_master_t *, ds4422_ds4424_i2c_slave_address_t);
    ds4422_ds4424_error_t ds4422_ds4424_out0_sink(ds4422_ds4424_t *, uint8_t);
    ds4422_ds4424_error_t ds4422_ds4424_out0_source(ds4422_ds4424_t *, uint8_t);
    ds4422_ds4424_error_t ds4422_ds4424_out1_sink(ds4422_ds4424_t *, uint8_t);
    ds4422_ds4424_error_t ds4422_ds4424_out1_source(ds4422_ds4424_t *, uint8_t);
    ds4422_ds4424_error_t ds4422_ds4424_out2_sink(ds4422_ds4424_t *, uint8_t);
    ds4422_ds4424_error_t ds4422_ds4424_out2_source(ds4422_ds4424_t *, uint8_t);
    ds4422_ds4424_error_t ds4422_ds4424_out3_sink(ds4422_ds4424_t *, uint8_t);
    ds4422_ds4424_error_t ds4422_ds4424_out3_source(ds4422_ds4424_t *, uint8_t);

#ifdef __cplusplus
}
#endif

#endif /* DS4422_DS4424_H */