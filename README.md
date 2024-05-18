# [DS4422](https://www.analog.com/products/ds4422.html)/[DS4424](https://www.analog.com/products/ds4424.html)

```c
#include <ds4422_ds4424.h>

typedef struct i2c_master_t
{
    ds4422_ds4424_i2c_master_t parent;
    /* ... */
} i2c_master_t;

ds4422_ds4424_error_t i2c_master_write(ds4422_ds4424_i2c_master_t *parent, ds4422_ds4424_i2c_slave_address_t slave_address, uint8_t data[], size_t size)
{
    i2c_master_t *i2c_master = (i2c_master_t *)parent;
    /* ... */
    return DS4422_DS4424_SUCCESS;
}

void i2c_master_init(i2c_master_t *i2c_master)
{
    i2c_master->parent.write = i2c_master_write;
    /* ... */
}

int main(void)
{
    i2c_master_t i2c_master;
    i2c_master_init(&i2c_master);

    ds4422_ds4424_t ds4422_ds4424;
    ds4422_ds4424_init(&ds4422_ds4424, (ds4422_ds4424_i2c_master_t *)&i2c_master, DS4422_DS4424_A0_GND_A1_GND);

    /* ... */
}
```