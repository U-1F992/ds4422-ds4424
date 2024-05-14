#ifndef DS4422_DS4424_H
#define DS4422_DS4424_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>

    typedef enum DS4422_DS4424Error
    {
        DS4422_DS4424_SUCCESS,
        DS4422_DS4424_ERROR_NULL_POINTER,
        DS4422_DS4424_ERROR_INVALID_SLAVE_ADDRESS,
        DS4422_DS4424_ERROR_INVALID_CURRENT_SOURCE,
        DS4422_DS4424_ERROR_INVALID_SIGN_BIT,
        DS4422_DS4424_ERROR_INVALID_DATA,
        DS4422_DS4424_ERROR_I2C_FAILURE
    } DS4422_DS4424Error;

    typedef enum DS4422_DS4424I2CSlaveAddress
    {
        DS4422_DS4424_A0_GND_A1_GND = 0x20, /* 0b0010000x */
        DS4422_DS4424_A0_VCC_A1_GND = 0x60, /* 0b0110000x */
        DS4422_DS4424_A0_GND_A1_VCC = 0xA0, /* 0b1010000x */
        DS4422_DS4424_A0_VCC_A1_VCC = 0xE0  /* 0b1110000x */
    } DS4422_DS4424I2CSlaveAddress;

    typedef struct DS4422_DS4424I2CMaster
    {
        DS4422_DS4424Error (*write)(struct DS4422_DS4424I2CMaster *, DS4422_DS4424I2CSlaveAddress, uint8_t[], size_t);
    } DS4422_DS4424I2CMaster;

    DS4422_DS4424Error ds4422_ds4424_i2c_master_write(DS4422_DS4424I2CMaster *, DS4422_DS4424I2CSlaveAddress, uint8_t[], size_t);

    typedef struct DS4422_DS4424
    {
        DS4422_DS4424I2CMaster *i2c_master;
        DS4422_DS4424I2CSlaveAddress slave_address;
    } DS4422_DS4424;

    DS4422_DS4424Error ds4422_ds4424_init(DS4422_DS4424 *, DS4422_DS4424I2CMaster *, DS4422_DS4424I2CSlaveAddress);
    DS4422_DS4424Error ds4422_ds4424_out0_sink(DS4422_DS4424 *, uint8_t);
    DS4422_DS4424Error ds4422_ds4424_out0_source(DS4422_DS4424 *, uint8_t);
    DS4422_DS4424Error ds4422_ds4424_out1_sink(DS4422_DS4424 *, uint8_t);
    DS4422_DS4424Error ds4422_ds4424_out1_source(DS4422_DS4424 *, uint8_t);
    DS4422_DS4424Error ds4422_ds4424_out2_sink(DS4422_DS4424 *, uint8_t);
    DS4422_DS4424Error ds4422_ds4424_out2_source(DS4422_DS4424 *, uint8_t);
    DS4422_DS4424Error ds4422_ds4424_out3_sink(DS4422_DS4424 *, uint8_t);
    DS4422_DS4424Error ds4422_ds4424_out3_source(DS4422_DS4424 *, uint8_t);

#ifdef __cplusplus
}
#endif

#endif /* DS4422_DS4424_H */