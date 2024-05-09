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

    static inline DS4422_DS4424Error ds4422_ds4424_i2c_master_write(DS4422_DS4424I2CMaster *writer, DS4422_DS4424I2CSlaveAddress slave_address, uint8_t data[], size_t size)
    {
        if (writer == NULL)
        {
            return DS4422_DS4424_ERROR_NULL_POINTER;
        }

        return writer->write(writer, slave_address, data, size);
    }

    typedef struct DS4422_DS4424
    {
        DS4422_DS4424I2CMaster *i2c_master;
        DS4422_DS4424I2CSlaveAddress slave_address;
    } DS4422_DS4424;

    static inline DS4422_DS4424Error ds4422_ds4424_init(DS4422_DS4424 *ds4422_ds4424, DS4422_DS4424I2CMaster *i2c_master, DS4422_DS4424I2CSlaveAddress slave_address)
    {
        if (ds4422_ds4424 == NULL ||
            i2c_master == NULL)
        {
            return DS4422_DS4424_ERROR_NULL_POINTER;
        }
        else if (!(slave_address == DS4422_DS4424_A0_GND_A1_GND ||
                   slave_address == DS4422_DS4424_A0_VCC_A1_GND ||
                   slave_address == DS4422_DS4424_A0_GND_A1_VCC ||
                   slave_address == DS4422_DS4424_A0_VCC_A1_VCC))
        {
            return DS4422_DS4424_ERROR_INVALID_SLAVE_ADDRESS;
        }

        ds4422_ds4424->i2c_master = i2c_master;
        ds4422_ds4424->slave_address = slave_address;

        return DS4422_DS4424_SUCCESS;
    }

    typedef enum DS4422_DS4424CurrentSources
    {
        DS4422_DS4424_OUT0 = 0xF8,
        DS4422_DS4424_OUT1 = 0xF9,
        DS4422_DS4424_OUT2 = 0xFA,
        DS4422_DS4424_OUT3 = 0xFB
    } DS4422_DS4424CurrentSources;

    typedef enum DS4422_DS4424SignBit
    {
        DS4422_DS4424_SINK = 0,
        DS4422_DS4424_SOURCE = 1
    } DS4422_DS4424SignBit;

    static inline DS4422_DS4424Error ds4422_ds4424_write(DS4422_DS4424 *ds4422_ds4424, DS4422_DS4424CurrentSources current_source, DS4422_DS4424SignBit sign_bit, uint8_t data)
    {
        if (ds4422_ds4424 == NULL)
        {
            return DS4422_DS4424_ERROR_NULL_POINTER;
        }
        else if (!(current_source == DS4422_DS4424_OUT0 ||
                   current_source == DS4422_DS4424_OUT1 ||
                   current_source == DS4422_DS4424_OUT2 ||
                   current_source == DS4422_DS4424_OUT3))
        {
            return DS4422_DS4424_ERROR_INVALID_CURRENT_SOURCE;
        }
        else if (!(sign_bit == DS4422_DS4424_SINK ||
                   sign_bit == DS4422_DS4424_SOURCE))
        {
            return DS4422_DS4424_ERROR_INVALID_SIGN_BIT;
        }
        else if (0x7F /* 0b1111111 */ < data)
        {
            return DS4422_DS4424_ERROR_INVALID_DATA;
        }

        return ds4422_ds4424_i2c_master_write(ds4422_ds4424->i2c_master,                             //
                                              ds4422_ds4424->slave_address,                          //
                                              (uint8_t[]){(uint8_t)current_source,                   //
                                                          (uint8_t)((uint8_t)sign_bit << 7 | data)}, //
                                              2);
    }

    static inline DS4422_DS4424Error ds4422_ds4424_out0_sink(DS4422_DS4424 *ds4422_ds4424, uint8_t data) { return ds4422_ds4424_write(ds4422_ds4424, DS4422_DS4424_OUT0, DS4422_DS4424_SINK, data); }
    static inline DS4422_DS4424Error ds4422_ds4424_out0_source(DS4422_DS4424 *ds4422_ds4424, uint8_t data) { return ds4422_ds4424_write(ds4422_ds4424, DS4422_DS4424_OUT0, DS4422_DS4424_SOURCE, data); }
    static inline DS4422_DS4424Error ds4422_ds4424_out1_sink(DS4422_DS4424 *ds4422_ds4424, uint8_t data) { return ds4422_ds4424_write(ds4422_ds4424, DS4422_DS4424_OUT1, DS4422_DS4424_SINK, data); }
    static inline DS4422_DS4424Error ds4422_ds4424_out1_source(DS4422_DS4424 *ds4422_ds4424, uint8_t data) { return ds4422_ds4424_write(ds4422_ds4424, DS4422_DS4424_OUT1, DS4422_DS4424_SOURCE, data); }
    static inline DS4422_DS4424Error ds4422_ds4424_out2_sink(DS4422_DS4424 *ds4422_ds4424, uint8_t data) { return ds4422_ds4424_write(ds4422_ds4424, DS4422_DS4424_OUT2, DS4422_DS4424_SINK, data); }
    static inline DS4422_DS4424Error ds4422_ds4424_out2_source(DS4422_DS4424 *ds4422_ds4424, uint8_t data) { return ds4422_ds4424_write(ds4422_ds4424, DS4422_DS4424_OUT2, DS4422_DS4424_SOURCE, data); }
    static inline DS4422_DS4424Error ds4422_ds4424_out3_sink(DS4422_DS4424 *ds4422_ds4424, uint8_t data) { return ds4422_ds4424_write(ds4422_ds4424, DS4422_DS4424_OUT3, DS4422_DS4424_SINK, data); }
    static inline DS4422_DS4424Error ds4422_ds4424_out3_source(DS4422_DS4424 *ds4422_ds4424, uint8_t data) { return ds4422_ds4424_write(ds4422_ds4424, DS4422_DS4424_OUT3, DS4422_DS4424_SOURCE, data); }

#ifdef __cplusplus
}
#endif

#endif /* DS4422_DS4424_H */