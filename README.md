# [DS4422](https://www.analog.com/products/ds4422.html)/[DS4424](https://www.analog.com/products/ds4424.html)

## Basic usage

```c
typedef struct I2CMaster
{
    DS4422_DS4424I2CMaster parent;
    /* ... */
} I2CMaster;

DS4422_DS4424Error i2c_master_write(DS4422_DS4424I2CMaster *parent, DS4422_DS4424I2CSlaveAddress slave_address, uint8_t data[], size_t size)
{
    I2CMaster *i2c_master = (I2CMaster *)parent;
    /* ... */
    return DS4422_DS4424_SUCCESS;
}

void i2c_master_init(I2CMaster *i2c_master)
{
    i2c_master->parent.write = i2c_master_write;
    /* ... */
}

int main(void)
{
    I2CMaster i2c_master;
    i2c_master_init(&i2c_master);

    DS4422_DS4424 ds4422_ds4424;
    ds4422_ds4424_init(&ds4422_ds4424, (DS4422_DS4424I2CMaster *)&i2c_master, DS4422_DS4424_A0_GND_A1_GND);

    /* ... */
}
```