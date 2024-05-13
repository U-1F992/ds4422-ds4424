#include <ds4422-ds4424.h>

DS4422_DS4424Error ds4422_ds4424_i2c_master_write(DS4422_DS4424I2CMaster *writer, DS4422_DS4424I2CSlaveAddress slave_address, uint8_t data[], size_t size)
{
    if (writer == NULL)
    {
        return DS4422_DS4424_ERROR_NULL_POINTER;
    }

    return writer->write(writer, slave_address, data, size);
}

DS4422_DS4424Error ds4422_ds4424_init(DS4422_DS4424 *ds4422_ds4424, DS4422_DS4424I2CMaster *i2c_master, DS4422_DS4424I2CSlaveAddress slave_address)
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

static DS4422_DS4424Error ds4422_ds4424_write(DS4422_DS4424 *ds4422_ds4424, DS4422_DS4424CurrentSources current_source, DS4422_DS4424SignBit sign_bit, uint8_t data)
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
    DS4422_DS4424Error err = ds4422_ds4424_i2c_master_write(ds4422_ds4424->i2c_master,    //
                                                            ds4422_ds4424->slave_address, //
                                                            data_,                        //
                                                            sizeof(data_) / sizeof(uint8_t));
    if (err != DS4422_DS4424_SUCCESS)
    {
        return DS4422_DS4424_ERROR_I2C_FAILURE;
    }

    return DS4422_DS4424_SUCCESS;
}

DS4422_DS4424Error ds4422_ds4424_out0_sink(DS4422_DS4424 *ds4422_ds4424, uint8_t data) { return ds4422_ds4424_write(ds4422_ds4424, DS4422_DS4424_OUT0, DS4422_DS4424_SINK, data); }
DS4422_DS4424Error ds4422_ds4424_out0_source(DS4422_DS4424 *ds4422_ds4424, uint8_t data) { return ds4422_ds4424_write(ds4422_ds4424, DS4422_DS4424_OUT0, DS4422_DS4424_SOURCE, data); }
DS4422_DS4424Error ds4422_ds4424_out1_sink(DS4422_DS4424 *ds4422_ds4424, uint8_t data) { return ds4422_ds4424_write(ds4422_ds4424, DS4422_DS4424_OUT1, DS4422_DS4424_SINK, data); }
DS4422_DS4424Error ds4422_ds4424_out1_source(DS4422_DS4424 *ds4422_ds4424, uint8_t data) { return ds4422_ds4424_write(ds4422_ds4424, DS4422_DS4424_OUT1, DS4422_DS4424_SOURCE, data); }
DS4422_DS4424Error ds4422_ds4424_out2_sink(DS4422_DS4424 *ds4422_ds4424, uint8_t data) { return ds4422_ds4424_write(ds4422_ds4424, DS4422_DS4424_OUT2, DS4422_DS4424_SINK, data); }
DS4422_DS4424Error ds4422_ds4424_out2_source(DS4422_DS4424 *ds4422_ds4424, uint8_t data) { return ds4422_ds4424_write(ds4422_ds4424, DS4422_DS4424_OUT2, DS4422_DS4424_SOURCE, data); }
DS4422_DS4424Error ds4422_ds4424_out3_sink(DS4422_DS4424 *ds4422_ds4424, uint8_t data) { return ds4422_ds4424_write(ds4422_ds4424, DS4422_DS4424_OUT3, DS4422_DS4424_SINK, data); }
DS4422_DS4424Error ds4422_ds4424_out3_source(DS4422_DS4424 *ds4422_ds4424, uint8_t data) { return ds4422_ds4424_write(ds4422_ds4424, DS4422_DS4424_OUT3, DS4422_DS4424_SOURCE, data); }