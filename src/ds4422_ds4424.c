#include <ds4422_ds4424.h>

ds4422_ds4424_error_t ds4422_ds4424_i2c_master_write(ds4422_ds4424_i2c_master_t *i2c_master, ds4422_ds4424_i2c_slave_address_t slave_address, uint8_t data[], size_t size)
{
    if (i2c_master == NULL)
    {
        return DS4422_DS4424_ERROR_NULL_POINTER;
    }

    return i2c_master->write(i2c_master, slave_address, data, size);
}

ds4422_ds4424_error_t ds4422_ds4424_init(ds4422_ds4424_t *ds4422_ds4424, ds4422_ds4424_i2c_master_t *i2c_master, ds4422_ds4424_i2c_slave_address_t slave_address)
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

typedef enum ds4422_ds4424_current_sources_t
{
    DS4422_DS4424_OUT0 = 0xF8,
    DS4422_DS4424_OUT1 = 0xF9,
    DS4422_DS4424_OUT2 = 0xFA,
    DS4422_DS4424_OUT3 = 0xFB
} ds4422_ds4424_current_sources_t;

typedef enum ds4422_ds4424_sign_bit_t
{
    DS4422_DS4424_SINK = 0,
    DS4422_DS4424_SOURCE = 1
} ds4422_ds4424_sign_bit_t;

static ds4422_ds4424_error_t ds4422_ds4424_write(ds4422_ds4424_t *ds4422_ds4424, ds4422_ds4424_current_sources_t current_source, ds4422_ds4424_sign_bit_t sign_bit, uint8_t data)
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

    uint8_t data_[] = {(uint8_t)current_source, //
                       (uint8_t)(((uint8_t)sign_bit) << 7 | data)};
    ds4422_ds4424_error_t err = ds4422_ds4424_i2c_master_write(ds4422_ds4424->i2c_master,    //
                                                               ds4422_ds4424->slave_address, //
                                                               data_,                        //
                                                               sizeof(data_) / sizeof(uint8_t));
    if (err != DS4422_DS4424_SUCCESS)
    {
        return DS4422_DS4424_ERROR_I2C_FAILURE;
    }

    return DS4422_DS4424_SUCCESS;
}

ds4422_ds4424_error_t ds4422_ds4424_out0_sink(ds4422_ds4424_t *ds4422_ds4424, uint8_t data) { return ds4422_ds4424_write(ds4422_ds4424, DS4422_DS4424_OUT0, DS4422_DS4424_SINK, data); }
ds4422_ds4424_error_t ds4422_ds4424_out0_source(ds4422_ds4424_t *ds4422_ds4424, uint8_t data) { return ds4422_ds4424_write(ds4422_ds4424, DS4422_DS4424_OUT0, DS4422_DS4424_SOURCE, data); }
ds4422_ds4424_error_t ds4422_ds4424_out1_sink(ds4422_ds4424_t *ds4422_ds4424, uint8_t data) { return ds4422_ds4424_write(ds4422_ds4424, DS4422_DS4424_OUT1, DS4422_DS4424_SINK, data); }
ds4422_ds4424_error_t ds4422_ds4424_out1_source(ds4422_ds4424_t *ds4422_ds4424, uint8_t data) { return ds4422_ds4424_write(ds4422_ds4424, DS4422_DS4424_OUT1, DS4422_DS4424_SOURCE, data); }
ds4422_ds4424_error_t ds4422_ds4424_out2_sink(ds4422_ds4424_t *ds4422_ds4424, uint8_t data) { return ds4422_ds4424_write(ds4422_ds4424, DS4422_DS4424_OUT2, DS4422_DS4424_SINK, data); }
ds4422_ds4424_error_t ds4422_ds4424_out2_source(ds4422_ds4424_t *ds4422_ds4424, uint8_t data) { return ds4422_ds4424_write(ds4422_ds4424, DS4422_DS4424_OUT2, DS4422_DS4424_SOURCE, data); }
ds4422_ds4424_error_t ds4422_ds4424_out3_sink(ds4422_ds4424_t *ds4422_ds4424, uint8_t data) { return ds4422_ds4424_write(ds4422_ds4424, DS4422_DS4424_OUT3, DS4422_DS4424_SINK, data); }
ds4422_ds4424_error_t ds4422_ds4424_out3_source(ds4422_ds4424_t *ds4422_ds4424, uint8_t data) { return ds4422_ds4424_write(ds4422_ds4424, DS4422_DS4424_OUT3, DS4422_DS4424_SOURCE, data); }