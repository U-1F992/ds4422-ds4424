#include <ds4422_ds4424.h>

#include <assert.h>
#include <stdio.h>

#define TEST_TEXT_RED "\e[31m"
#define TEST_TEXT_GREEN "\e[32m"
#define TEST_TEXT_RESET "\e[0m"

#define TEST_INVALID_ADDRESS (0xCAFE)

typedef struct test_i2c_master_t
{
    ds4422_ds4424_i2c_master_t parent;
    ds4422_ds4424_i2c_slave_address_t last_written_address;
    size_t recent_size;
    uint8_t recent_data[128];
} test_i2c_master_t;

ds4422_ds4424_error_t test_i2c_master_write(ds4422_ds4424_i2c_master_t *parent, ds4422_ds4424_i2c_slave_address_t slave_address, uint8_t data[], size_t size)
{
    test_i2c_master_t *i2c_master = (test_i2c_master_t *)parent;
    assert(i2c_master != NULL);

    i2c_master->last_written_address = slave_address;
    i2c_master->recent_size = size;
    for (size_t i = 0; i < size; i++)
    {
        i2c_master->recent_data[i] = data[i];
    }

    return DS4422_DS4424_SUCCESS;
}

void test_i2c_master_init(test_i2c_master_t *i2c_master)
{
    assert(i2c_master != NULL);

    i2c_master->parent.write = test_i2c_master_write;
}

int test_ds4422_ds4424_init(void)
{
    printf("* %s\n", __func__);
    int test_failure_count = 0;

    typedef struct test_case_t
    {
        ds4422_ds4424_t *ds4422_ds4424;
        ds4422_ds4424_i2c_master_t *i2c_master;
        ds4422_ds4424_i2c_slave_address_t slave_address;

        ds4422_ds4424_error_t expected_ret;
    } test_case_t;

    ds4422_ds4424_t ds4422_ds4424[20];
    ds4422_ds4424_i2c_master_t i2c_master[20];

    test_case_t test_cases[] = {{.ds4422_ds4424 = NULL, /*              */ .i2c_master = NULL, /*           */ .slave_address = TEST_INVALID_ADDRESS, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[1], /* */ .i2c_master = NULL, /*           */ .slave_address = TEST_INVALID_ADDRESS, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[2], /* */ .slave_address = TEST_INVALID_ADDRESS, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[3], /* */ .i2c_master = &i2c_master[3], /* */ .slave_address = TEST_INVALID_ADDRESS, /*  */ .expected_ret = DS4422_DS4424_ERROR_INVALID_SLAVE_ADDRESS},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = NULL, /*           */ .slave_address = DS4422_DS4424_A0_GND_A1_GND, .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[5], /* */ .i2c_master = NULL, /*           */ .slave_address = DS4422_DS4424_A0_GND_A1_GND, .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[6], /* */ .slave_address = DS4422_DS4424_A0_GND_A1_GND, .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[7], /* */ .i2c_master = &i2c_master[7], /* */ .slave_address = DS4422_DS4424_A0_GND_A1_GND, .expected_ret = DS4422_DS4424_SUCCESS},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = NULL, /*           */ .slave_address = DS4422_DS4424_A0_VCC_A1_GND, .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[9], /* */ .i2c_master = NULL, /*           */ .slave_address = DS4422_DS4424_A0_VCC_A1_GND, .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[10], /**/ .slave_address = DS4422_DS4424_A0_VCC_A1_GND, .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[11], /**/ .i2c_master = &i2c_master[11], /**/ .slave_address = DS4422_DS4424_A0_VCC_A1_GND, .expected_ret = DS4422_DS4424_SUCCESS},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = NULL, /*           */ .slave_address = DS4422_DS4424_A0_GND_A1_VCC, .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[13], /**/ .i2c_master = NULL, /*           */ .slave_address = DS4422_DS4424_A0_GND_A1_VCC, .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[14], /**/ .slave_address = DS4422_DS4424_A0_GND_A1_VCC, .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[15], /**/ .i2c_master = &i2c_master[15], /**/ .slave_address = DS4422_DS4424_A0_GND_A1_VCC, .expected_ret = DS4422_DS4424_SUCCESS},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = NULL, /*           */ .slave_address = DS4422_DS4424_A0_VCC_A1_VCC, .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[17], /**/ .i2c_master = NULL, /*           */ .slave_address = DS4422_DS4424_A0_VCC_A1_VCC, .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[18], /**/ .slave_address = DS4422_DS4424_A0_VCC_A1_VCC, .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[19], /**/ .i2c_master = &i2c_master[19], /**/ .slave_address = DS4422_DS4424_A0_VCC_A1_VCC, .expected_ret = DS4422_DS4424_SUCCESS}};
    size_t test_size = sizeof(test_cases) / sizeof(test_case_t);

    for (size_t i = 0; i < test_size; i++)
    {
        test_case_t *test_case = &test_cases[i];

        ds4422_ds4424_error_t actual_ret = ds4422_ds4424_init(test_case->ds4422_ds4424, test_case->i2c_master, test_case->slave_address);
        if (test_case->expected_ret != actual_ret)
        {
            fprintf(stderr, "%sindex: %d, expected_ret: %d, actual_ret: %d%s\n", TEST_TEXT_RED, i, test_case->expected_ret, actual_ret, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }
    }

    return test_failure_count;
}

int test_ds4422_ds4424_out0_sink(void)
{
    printf("* %s\n", __func__);
    int test_failure_count = 0;

    typedef struct test_case_t
    {
        ds4422_ds4424_t *ds4422_ds4424;
        test_i2c_master_t *i2c_master;
        uint8_t data;

        ds4422_ds4424_error_t expected_ret;
        ds4422_ds4424_i2c_slave_address_t expected_slave_address;
        size_t expected_size;
        uint8_t *expected_data;
    } test_case_t;

    ds4422_ds4424_t ds4422_ds4424[24];
    test_i2c_master_t i2c_master[24];
    for (size_t i = 0; i < 24; i++)
    {
        test_i2c_master_init(&i2c_master[i]);
        ds4422_ds4424_error_t err = ds4422_ds4424_init(&ds4422_ds4424[i], (ds4422_ds4424_i2c_master_t *)&i2c_master[i], (0 <= i && i < 6)     ? DS4422_DS4424_A0_GND_A1_GND //
                                                                                                                        : (6 <= i && i < 12)  ? DS4422_DS4424_A0_VCC_A1_GND //
                                                                                                                        : (12 <= i && i < 18) ? DS4422_DS4424_A0_GND_A1_VCC //
                                                                                                                                              : DS4422_DS4424_A0_VCC_A1_VCC);
        assert(err == DS4422_DS4424_SUCCESS);
    }

    test_case_t test_cases[] = {{.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[0], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[1], /* */ .i2c_master = &i2c_master[1], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xF8, 0x00}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[2], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[3], /* */ .i2c_master = &i2c_master[3], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xF8, 0x7F}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[4], /* */ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[5], /* */ .i2c_master = &i2c_master[5], /* */ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[6], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[7], /* */ .i2c_master = &i2c_master[7], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xF8, 0x00}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[8], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[9], /* */ .i2c_master = &i2c_master[9], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xF8, 0x7F}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[10], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[11], /**/ .i2c_master = &i2c_master[11], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[12], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[13], /**/ .i2c_master = &i2c_master[13], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xF8, 0x00}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[14], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[15], /**/ .i2c_master = &i2c_master[15], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xF8, 0x7F}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[16], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[17], /**/ .i2c_master = &i2c_master[17], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[18], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[19], /**/ .i2c_master = &i2c_master[19], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xF8, 0x00}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[20], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[21], /**/ .i2c_master = &i2c_master[21], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xF8, 0x7F}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[22], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[23], /**/ .i2c_master = &i2c_master[23], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA}};
    size_t test_size = sizeof(test_cases) / sizeof(test_case_t);

    for (size_t i = 0; i < test_size; i++)
    {
        test_case_t *test_case = &test_cases[i];

        ds4422_ds4424_error_t actual_ret = ds4422_ds4424_out0_sink(test_case->ds4422_ds4424, test_case->data);
        if (test_case->expected_ret != actual_ret)
        {
            fprintf(stderr, "%sindex: %d, expected_ret: %d, actual_ret: %d%s\n", TEST_TEXT_RED, i, test_case->expected_ret, actual_ret, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }
        else if (actual_ret != 0)
        {
            continue;
        }

        ds4422_ds4424_i2c_slave_address_t actual_slave_address = test_case->i2c_master->last_written_address;
        if (test_case->expected_slave_address != actual_slave_address)
        {
            fprintf(stderr, "%sindex: %d, expected_slave_address: %d, actual_slave_address: %d%s\n", TEST_TEXT_RED, i, test_case->expected_slave_address, actual_slave_address, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }

        size_t actual_size = test_case->i2c_master->recent_size;
        if (test_case->expected_size != actual_size)
        {
            fprintf(stderr, "%sindex: %d, expected_size: %d, actual_size: %d%s\n", TEST_TEXT_RED, i, test_case->expected_size, actual_size, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }

        uint8_t *actual_data = test_case->i2c_master->recent_data;
        for (size_t j = 0; j < actual_size; j++)
        {
            if (test_case->expected_data[j] != actual_data[j])
            {
                fprintf(stderr, "%sindex: %d, expected_data[%d]: %d, actual_data[%d]: %d%s\n", TEST_TEXT_RED, i, j, test_case->expected_data[j], j, actual_data[j], TEST_TEXT_RESET);
                test_failure_count++;
                continue;
            }
        }
    }

    return test_failure_count;
}

int test_ds4422_ds4424_out0_source(void)
{
    printf("* %s\n", __func__);
    int test_failure_count = 0;

    typedef struct test_case_t
    {
        ds4422_ds4424_t *ds4422_ds4424;
        test_i2c_master_t *i2c_master;
        uint8_t data;

        ds4422_ds4424_error_t expected_ret;
        ds4422_ds4424_i2c_slave_address_t expected_slave_address;
        size_t expected_size;
        uint8_t *expected_data;
    } test_case_t;

    ds4422_ds4424_t ds4422_ds4424[24];
    test_i2c_master_t i2c_master[24];
    for (size_t i = 0; i < 24; i++)
    {
        test_i2c_master_init(&i2c_master[i]);
        ds4422_ds4424_error_t err = ds4422_ds4424_init(&ds4422_ds4424[i], (ds4422_ds4424_i2c_master_t *)&i2c_master[i], (0 <= i && i < 6)     ? DS4422_DS4424_A0_GND_A1_GND //
                                                                                                                        : (6 <= i && i < 12)  ? DS4422_DS4424_A0_VCC_A1_GND //
                                                                                                                        : (12 <= i && i < 18) ? DS4422_DS4424_A0_GND_A1_VCC //
                                                                                                                                              : DS4422_DS4424_A0_VCC_A1_VCC);
        assert(err == DS4422_DS4424_SUCCESS);
    }

    test_case_t test_cases[] = {{.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[0], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[1], /* */ .i2c_master = &i2c_master[1], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xF8, 0x80}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[2], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[3], /* */ .i2c_master = &i2c_master[3], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xF8, 0xFF}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[4], /* */ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[5], /* */ .i2c_master = &i2c_master[5], /* */ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[6], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[7], /* */ .i2c_master = &i2c_master[7], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xF8, 0x80}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[8], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[9], /* */ .i2c_master = &i2c_master[9], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xF8, 0xFF}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[10], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[11], /**/ .i2c_master = &i2c_master[11], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[12], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[13], /**/ .i2c_master = &i2c_master[13], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xF8, 0x80}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[14], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[15], /**/ .i2c_master = &i2c_master[15], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xF8, 0xFF}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[16], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[17], /**/ .i2c_master = &i2c_master[17], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[18], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[19], /**/ .i2c_master = &i2c_master[19], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xF8, 0x80}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[20], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[21], /**/ .i2c_master = &i2c_master[21], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xF8, 0xFF}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[22], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[23], /**/ .i2c_master = &i2c_master[23], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA}};
    size_t test_size = sizeof(test_cases) / sizeof(test_case_t);

    for (size_t i = 0; i < test_size; i++)
    {
        test_case_t *test_case = &test_cases[i];

        ds4422_ds4424_error_t actual_ret = ds4422_ds4424_out0_source(test_case->ds4422_ds4424, test_case->data);
        if (test_case->expected_ret != actual_ret)
        {
            fprintf(stderr, "%sindex: %d, expected_ret: %d, actual_ret: %d%s\n", TEST_TEXT_RED, i, test_case->expected_ret, actual_ret, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }
        else if (actual_ret != 0)
        {
            continue;
        }

        ds4422_ds4424_i2c_slave_address_t actual_slave_address = test_case->i2c_master->last_written_address;
        if (test_case->expected_slave_address != actual_slave_address)
        {
            fprintf(stderr, "%sindex: %d, expected_slave_address: %d, actual_slave_address: %d%s\n", TEST_TEXT_RED, i, test_case->expected_slave_address, actual_slave_address, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }

        size_t actual_size = test_case->i2c_master->recent_size;
        if (test_case->expected_size != actual_size)
        {
            fprintf(stderr, "%sindex: %d, expected_size: %d, actual_size: %d%s\n", TEST_TEXT_RED, i, test_case->expected_size, actual_size, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }

        uint8_t *actual_data = test_case->i2c_master->recent_data;
        for (size_t j = 0; j < actual_size; j++)
        {
            if (test_case->expected_data[j] != actual_data[j])
            {
                fprintf(stderr, "%sindex: %d, expected_data[%d]: %d, actual_data[%d]: %d%s\n", TEST_TEXT_RED, i, j, test_case->expected_data[j], j, actual_data[j], TEST_TEXT_RESET);
                test_failure_count++;
                continue;
            }
        }
    }

    return test_failure_count;
}

int test_ds4422_ds4424_out1_sink(void)
{
    printf("* %s\n", __func__);
    int test_failure_count = 0;

    typedef struct test_case_t
    {
        ds4422_ds4424_t *ds4422_ds4424;
        test_i2c_master_t *i2c_master;
        uint8_t data;

        ds4422_ds4424_error_t expected_ret;
        ds4422_ds4424_i2c_slave_address_t expected_slave_address;
        size_t expected_size;
        uint8_t *expected_data;
    } test_case_t;

    ds4422_ds4424_t ds4422_ds4424[24];
    test_i2c_master_t i2c_master[24];
    for (size_t i = 0; i < 24; i++)
    {
        test_i2c_master_init(&i2c_master[i]);
        ds4422_ds4424_error_t err = ds4422_ds4424_init(&ds4422_ds4424[i], (ds4422_ds4424_i2c_master_t *)&i2c_master[i], (0 <= i && i < 6)     ? DS4422_DS4424_A0_GND_A1_GND //
                                                                                                                        : (6 <= i && i < 12)  ? DS4422_DS4424_A0_VCC_A1_GND //
                                                                                                                        : (12 <= i && i < 18) ? DS4422_DS4424_A0_GND_A1_VCC //
                                                                                                                                              : DS4422_DS4424_A0_VCC_A1_VCC);
        assert(err == DS4422_DS4424_SUCCESS);
    }

    test_case_t test_cases[] = {{.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[0], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[1], /* */ .i2c_master = &i2c_master[1], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xF9, 0x00}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[2], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[3], /* */ .i2c_master = &i2c_master[3], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xF9, 0x7F}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[4], /* */ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[5], /* */ .i2c_master = &i2c_master[5], /* */ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[6], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[7], /* */ .i2c_master = &i2c_master[7], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xF9, 0x00}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[8], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[9], /* */ .i2c_master = &i2c_master[9], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xF9, 0x7F}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[10], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[11], /**/ .i2c_master = &i2c_master[11], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[12], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[13], /**/ .i2c_master = &i2c_master[13], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xF9, 0x00}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[14], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[15], /**/ .i2c_master = &i2c_master[15], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xF9, 0x7F}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[16], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[17], /**/ .i2c_master = &i2c_master[17], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[18], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[19], /**/ .i2c_master = &i2c_master[19], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xF9, 0x00}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[20], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[21], /**/ .i2c_master = &i2c_master[21], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xF9, 0x7F}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[22], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[23], /**/ .i2c_master = &i2c_master[23], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA}};
    size_t test_size = sizeof(test_cases) / sizeof(test_case_t);

    for (size_t i = 0; i < test_size; i++)
    {
        test_case_t *test_case = &test_cases[i];

        ds4422_ds4424_error_t actual_ret = ds4422_ds4424_out1_sink(test_case->ds4422_ds4424, test_case->data);
        if (test_case->expected_ret != actual_ret)
        {
            fprintf(stderr, "%sindex: %d, expected_ret: %d, actual_ret: %d%s\n", TEST_TEXT_RED, i, test_case->expected_ret, actual_ret, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }
        else if (actual_ret != 0)
        {
            continue;
        }

        ds4422_ds4424_i2c_slave_address_t actual_slave_address = test_case->i2c_master->last_written_address;
        if (test_case->expected_slave_address != actual_slave_address)
        {
            fprintf(stderr, "%sindex: %d, expected_slave_address: %d, actual_slave_address: %d%s\n", TEST_TEXT_RED, i, test_case->expected_slave_address, actual_slave_address, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }

        size_t actual_size = test_case->i2c_master->recent_size;
        if (test_case->expected_size != actual_size)
        {
            fprintf(stderr, "%sindex: %d, expected_size: %d, actual_size: %d%s\n", TEST_TEXT_RED, i, test_case->expected_size, actual_size, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }

        uint8_t *actual_data = test_case->i2c_master->recent_data;
        for (size_t j = 0; j < actual_size; j++)
        {
            if (test_case->expected_data[j] != actual_data[j])
            {
                fprintf(stderr, "%sindex: %d, expected_data[%d]: %d, actual_data[%d]: %d%s\n", TEST_TEXT_RED, i, j, test_case->expected_data[j], j, actual_data[j], TEST_TEXT_RESET);
                test_failure_count++;
                continue;
            }
        }
    }

    return test_failure_count;
}

int test_ds4422_ds4424_out1_source(void)
{
    printf("* %s\n", __func__);
    int test_failure_count = 0;

    typedef struct test_case_t
    {
        ds4422_ds4424_t *ds4422_ds4424;
        test_i2c_master_t *i2c_master;
        uint8_t data;

        ds4422_ds4424_error_t expected_ret;
        ds4422_ds4424_i2c_slave_address_t expected_slave_address;
        size_t expected_size;
        uint8_t *expected_data;
    } test_case_t;

    ds4422_ds4424_t ds4422_ds4424[24];
    test_i2c_master_t i2c_master[24];
    for (size_t i = 0; i < 24; i++)
    {
        test_i2c_master_init(&i2c_master[i]);
        ds4422_ds4424_error_t err = ds4422_ds4424_init(&ds4422_ds4424[i], (ds4422_ds4424_i2c_master_t *)&i2c_master[i], (0 <= i && i < 6)     ? DS4422_DS4424_A0_GND_A1_GND //
                                                                                                                        : (6 <= i && i < 12)  ? DS4422_DS4424_A0_VCC_A1_GND //
                                                                                                                        : (12 <= i && i < 18) ? DS4422_DS4424_A0_GND_A1_VCC //
                                                                                                                                              : DS4422_DS4424_A0_VCC_A1_VCC);
        assert(err == DS4422_DS4424_SUCCESS);
    }

    test_case_t test_cases[] = {{.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[0], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[1], /* */ .i2c_master = &i2c_master[1], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xF9, 0x80}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[2], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[3], /* */ .i2c_master = &i2c_master[3], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xF9, 0xFF}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[4], /* */ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[5], /* */ .i2c_master = &i2c_master[5], /* */ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[6], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[7], /* */ .i2c_master = &i2c_master[7], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xF9, 0x80}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[8], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[9], /* */ .i2c_master = &i2c_master[9], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xF9, 0xFF}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[10], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[11], /**/ .i2c_master = &i2c_master[11], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[12], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[13], /**/ .i2c_master = &i2c_master[13], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xF9, 0x80}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[14], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[15], /**/ .i2c_master = &i2c_master[15], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xF9, 0xFF}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[16], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[17], /**/ .i2c_master = &i2c_master[17], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[18], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[19], /**/ .i2c_master = &i2c_master[19], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xF9, 0x80}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[20], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[21], /**/ .i2c_master = &i2c_master[21], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xF9, 0xFF}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[22], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[23], /**/ .i2c_master = &i2c_master[23], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA}};
    size_t test_size = sizeof(test_cases) / sizeof(test_case_t);

    for (size_t i = 0; i < test_size; i++)
    {
        test_case_t *test_case = &test_cases[i];

        ds4422_ds4424_error_t actual_ret = ds4422_ds4424_out1_source(test_case->ds4422_ds4424, test_case->data);
        if (test_case->expected_ret != actual_ret)
        {
            fprintf(stderr, "%sindex: %d, expected_ret: %d, actual_ret: %d%s\n", TEST_TEXT_RED, i, test_case->expected_ret, actual_ret, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }
        else if (actual_ret != 0)
        {
            continue;
        }

        ds4422_ds4424_i2c_slave_address_t actual_slave_address = test_case->i2c_master->last_written_address;
        if (test_case->expected_slave_address != actual_slave_address)
        {
            fprintf(stderr, "%sindex: %d, expected_slave_address: %d, actual_slave_address: %d%s\n", TEST_TEXT_RED, i, test_case->expected_slave_address, actual_slave_address, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }

        size_t actual_size = test_case->i2c_master->recent_size;
        if (test_case->expected_size != actual_size)
        {
            fprintf(stderr, "%sindex: %d, expected_size: %d, actual_size: %d%s\n", TEST_TEXT_RED, i, test_case->expected_size, actual_size, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }

        uint8_t *actual_data = test_case->i2c_master->recent_data;
        for (size_t j = 0; j < actual_size; j++)
        {
            if (test_case->expected_data[j] != actual_data[j])
            {
                fprintf(stderr, "%sindex: %d, expected_data[%d]: %d, actual_data[%d]: %d%s\n", TEST_TEXT_RED, i, j, test_case->expected_data[j], j, actual_data[j], TEST_TEXT_RESET);
                test_failure_count++;
                continue;
            }
        }
    }

    return test_failure_count;
}

int test_ds4422_ds4424_out2_sink(void)
{
    printf("* %s\n", __func__);
    int test_failure_count = 0;

    typedef struct test_case_t
    {
        ds4422_ds4424_t *ds4422_ds4424;
        test_i2c_master_t *i2c_master;
        uint8_t data;

        ds4422_ds4424_error_t expected_ret;
        ds4422_ds4424_i2c_slave_address_t expected_slave_address;
        size_t expected_size;
        uint8_t *expected_data;
    } test_case_t;

    ds4422_ds4424_t ds4422_ds4424[24];
    test_i2c_master_t i2c_master[24];
    for (size_t i = 0; i < 24; i++)
    {
        test_i2c_master_init(&i2c_master[i]);
        ds4422_ds4424_error_t err = ds4422_ds4424_init(&ds4422_ds4424[i], (ds4422_ds4424_i2c_master_t *)&i2c_master[i], (0 <= i && i < 6)     ? DS4422_DS4424_A0_GND_A1_GND //
                                                                                                                        : (6 <= i && i < 12)  ? DS4422_DS4424_A0_VCC_A1_GND //
                                                                                                                        : (12 <= i && i < 18) ? DS4422_DS4424_A0_GND_A1_VCC //
                                                                                                                                              : DS4422_DS4424_A0_VCC_A1_VCC);
        assert(err == DS4422_DS4424_SUCCESS);
    }

    test_case_t test_cases[] = {{.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[0], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[1], /* */ .i2c_master = &i2c_master[1], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xFA, 0x00}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[2], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[3], /* */ .i2c_master = &i2c_master[3], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xFA, 0x7F}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[4], /* */ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[5], /* */ .i2c_master = &i2c_master[5], /* */ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[6], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[7], /* */ .i2c_master = &i2c_master[7], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xFA, 0x00}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[8], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[9], /* */ .i2c_master = &i2c_master[9], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xFA, 0x7F}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[10], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[11], /**/ .i2c_master = &i2c_master[11], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[12], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[13], /**/ .i2c_master = &i2c_master[13], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xFA, 0x00}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[14], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[15], /**/ .i2c_master = &i2c_master[15], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xFA, 0x7F}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[16], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[17], /**/ .i2c_master = &i2c_master[17], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[18], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[19], /**/ .i2c_master = &i2c_master[19], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xFA, 0x00}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[20], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[21], /**/ .i2c_master = &i2c_master[21], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xFA, 0x7F}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[22], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[23], /**/ .i2c_master = &i2c_master[23], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA}};
    size_t test_size = sizeof(test_cases) / sizeof(test_case_t);

    for (size_t i = 0; i < test_size; i++)
    {
        test_case_t *test_case = &test_cases[i];

        ds4422_ds4424_error_t actual_ret = ds4422_ds4424_out2_sink(test_case->ds4422_ds4424, test_case->data);
        if (test_case->expected_ret != actual_ret)
        {
            fprintf(stderr, "%sindex: %d, expected_ret: %d, actual_ret: %d%s\n", TEST_TEXT_RED, i, test_case->expected_ret, actual_ret, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }
        else if (actual_ret != 0)
        {
            continue;
        }

        ds4422_ds4424_i2c_slave_address_t actual_slave_address = test_case->i2c_master->last_written_address;
        if (test_case->expected_slave_address != actual_slave_address)
        {
            fprintf(stderr, "%sindex: %d, expected_slave_address: %d, actual_slave_address: %d%s\n", TEST_TEXT_RED, i, test_case->expected_slave_address, actual_slave_address, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }

        size_t actual_size = test_case->i2c_master->recent_size;
        if (test_case->expected_size != actual_size)
        {
            fprintf(stderr, "%sindex: %d, expected_size: %d, actual_size: %d%s\n", TEST_TEXT_RED, i, test_case->expected_size, actual_size, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }

        uint8_t *actual_data = test_case->i2c_master->recent_data;
        for (size_t j = 0; j < actual_size; j++)
        {
            if (test_case->expected_data[j] != actual_data[j])
            {
                fprintf(stderr, "%sindex: %d, expected_data[%d]: %d, actual_data[%d]: %d%s\n", TEST_TEXT_RED, i, j, test_case->expected_data[j], j, actual_data[j], TEST_TEXT_RESET);
                test_failure_count++;
                continue;
            }
        }
    }

    return test_failure_count;
}

int test_ds4422_ds4424_out2_source(void)
{
    printf("* %s\n", __func__);
    int test_failure_count = 0;

    typedef struct test_case_t
    {
        ds4422_ds4424_t *ds4422_ds4424;
        test_i2c_master_t *i2c_master;
        uint8_t data;

        ds4422_ds4424_error_t expected_ret;
        ds4422_ds4424_i2c_slave_address_t expected_slave_address;
        size_t expected_size;
        uint8_t *expected_data;
    } test_case_t;

    ds4422_ds4424_t ds4422_ds4424[24];
    test_i2c_master_t i2c_master[24];
    for (size_t i = 0; i < 24; i++)
    {
        test_i2c_master_init(&i2c_master[i]);
        ds4422_ds4424_error_t err = ds4422_ds4424_init(&ds4422_ds4424[i], (ds4422_ds4424_i2c_master_t *)&i2c_master[i], (0 <= i && i < 6)     ? DS4422_DS4424_A0_GND_A1_GND //
                                                                                                                        : (6 <= i && i < 12)  ? DS4422_DS4424_A0_VCC_A1_GND //
                                                                                                                        : (12 <= i && i < 18) ? DS4422_DS4424_A0_GND_A1_VCC //
                                                                                                                                              : DS4422_DS4424_A0_VCC_A1_VCC);
        assert(err == DS4422_DS4424_SUCCESS);
    }

    test_case_t test_cases[] = {{.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[0], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[1], /* */ .i2c_master = &i2c_master[1], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xFA, 0x80}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[2], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[3], /* */ .i2c_master = &i2c_master[3], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xFA, 0xFF}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[4], /* */ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[5], /* */ .i2c_master = &i2c_master[5], /* */ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[6], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[7], /* */ .i2c_master = &i2c_master[7], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xFA, 0x80}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[8], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[9], /* */ .i2c_master = &i2c_master[9], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xFA, 0xFF}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[10], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[11], /**/ .i2c_master = &i2c_master[11], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[12], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[13], /**/ .i2c_master = &i2c_master[13], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xFA, 0x80}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[14], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[15], /**/ .i2c_master = &i2c_master[15], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xFA, 0xFF}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[16], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[17], /**/ .i2c_master = &i2c_master[17], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[18], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[19], /**/ .i2c_master = &i2c_master[19], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xFA, 0x80}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[20], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[21], /**/ .i2c_master = &i2c_master[21], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xFA, 0xFF}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[22], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[23], /**/ .i2c_master = &i2c_master[23], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA}};
    size_t test_size = sizeof(test_cases) / sizeof(test_case_t);

    for (size_t i = 0; i < test_size; i++)
    {
        test_case_t *test_case = &test_cases[i];

        ds4422_ds4424_error_t actual_ret = ds4422_ds4424_out2_source(test_case->ds4422_ds4424, test_case->data);
        if (test_case->expected_ret != actual_ret)
        {
            fprintf(stderr, "%sindex: %d, expected_ret: %d, actual_ret: %d%s\n", TEST_TEXT_RED, i, test_case->expected_ret, actual_ret, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }
        else if (actual_ret != 0)
        {
            continue;
        }

        ds4422_ds4424_i2c_slave_address_t actual_slave_address = test_case->i2c_master->last_written_address;
        if (test_case->expected_slave_address != actual_slave_address)
        {
            fprintf(stderr, "%sindex: %d, expected_slave_address: %d, actual_slave_address: %d%s\n", TEST_TEXT_RED, i, test_case->expected_slave_address, actual_slave_address, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }

        size_t actual_size = test_case->i2c_master->recent_size;
        if (test_case->expected_size != actual_size)
        {
            fprintf(stderr, "%sindex: %d, expected_size: %d, actual_size: %d%s\n", TEST_TEXT_RED, i, test_case->expected_size, actual_size, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }

        uint8_t *actual_data = test_case->i2c_master->recent_data;
        for (size_t j = 0; j < actual_size; j++)
        {
            if (test_case->expected_data[j] != actual_data[j])
            {
                fprintf(stderr, "%sindex: %d, expected_data[%d]: %d, actual_data[%d]: %d%s\n", TEST_TEXT_RED, i, j, test_case->expected_data[j], j, actual_data[j], TEST_TEXT_RESET);
                test_failure_count++;
                continue;
            }
        }
    }

    return test_failure_count;
}

int test_ds4422_ds4424_out3_sink(void)
{
    printf("* %s\n", __func__);
    int test_failure_count = 0;

    typedef struct test_case_t
    {
        ds4422_ds4424_t *ds4422_ds4424;
        test_i2c_master_t *i2c_master;
        uint8_t data;

        ds4422_ds4424_error_t expected_ret;
        ds4422_ds4424_i2c_slave_address_t expected_slave_address;
        size_t expected_size;
        uint8_t *expected_data;
    } test_case_t;

    ds4422_ds4424_t ds4422_ds4424[24];
    test_i2c_master_t i2c_master[24];
    for (size_t i = 0; i < 24; i++)
    {
        test_i2c_master_init(&i2c_master[i]);
        ds4422_ds4424_error_t err = ds4422_ds4424_init(&ds4422_ds4424[i], (ds4422_ds4424_i2c_master_t *)&i2c_master[i], (0 <= i && i < 6)     ? DS4422_DS4424_A0_GND_A1_GND //
                                                                                                                        : (6 <= i && i < 12)  ? DS4422_DS4424_A0_VCC_A1_GND //
                                                                                                                        : (12 <= i && i < 18) ? DS4422_DS4424_A0_GND_A1_VCC //
                                                                                                                                              : DS4422_DS4424_A0_VCC_A1_VCC);
        assert(err == DS4422_DS4424_SUCCESS);
    }

    test_case_t test_cases[] = {{.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[0], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[1], /* */ .i2c_master = &i2c_master[1], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xFB, 0x00}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[2], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[3], /* */ .i2c_master = &i2c_master[3], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xFB, 0x7F}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[4], /* */ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[5], /* */ .i2c_master = &i2c_master[5], /* */ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[6], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[7], /* */ .i2c_master = &i2c_master[7], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xFB, 0x00}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[8], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[9], /* */ .i2c_master = &i2c_master[9], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xFB, 0x7F}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[10], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[11], /**/ .i2c_master = &i2c_master[11], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[12], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[13], /**/ .i2c_master = &i2c_master[13], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xFB, 0x00}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[14], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[15], /**/ .i2c_master = &i2c_master[15], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xFB, 0x7F}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[16], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[17], /**/ .i2c_master = &i2c_master[17], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[18], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[19], /**/ .i2c_master = &i2c_master[19], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xFB, 0x00}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[20], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[21], /**/ .i2c_master = &i2c_master[21], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xFB, 0x7F}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[22], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[23], /**/ .i2c_master = &i2c_master[23], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA}};
    size_t test_size = sizeof(test_cases) / sizeof(test_case_t);

    for (size_t i = 0; i < test_size; i++)
    {
        test_case_t *test_case = &test_cases[i];

        ds4422_ds4424_error_t actual_ret = ds4422_ds4424_out3_sink(test_case->ds4422_ds4424, test_case->data);
        if (test_case->expected_ret != actual_ret)
        {
            fprintf(stderr, "%sindex: %d, expected_ret: %d, actual_ret: %d%s\n", TEST_TEXT_RED, i, test_case->expected_ret, actual_ret, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }
        else if (actual_ret != 0)
        {
            continue;
        }

        ds4422_ds4424_i2c_slave_address_t actual_slave_address = test_case->i2c_master->last_written_address;
        if (test_case->expected_slave_address != actual_slave_address)
        {
            fprintf(stderr, "%sindex: %d, expected_slave_address: %d, actual_slave_address: %d%s\n", TEST_TEXT_RED, i, test_case->expected_slave_address, actual_slave_address, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }

        size_t actual_size = test_case->i2c_master->recent_size;
        if (test_case->expected_size != actual_size)
        {
            fprintf(stderr, "%sindex: %d, expected_size: %d, actual_size: %d%s\n", TEST_TEXT_RED, i, test_case->expected_size, actual_size, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }

        uint8_t *actual_data = test_case->i2c_master->recent_data;
        for (size_t j = 0; j < actual_size; j++)
        {
            if (test_case->expected_data[j] != actual_data[j])
            {
                fprintf(stderr, "%sindex: %d, expected_data[%d]: %d, actual_data[%d]: %d%s\n", TEST_TEXT_RED, i, j, test_case->expected_data[j], j, actual_data[j], TEST_TEXT_RESET);
                test_failure_count++;
                continue;
            }
        }
    }

    return test_failure_count;
}

int test_ds4422_ds4424_out3_source(void)
{
    printf("* %s\n", __func__);
    int test_failure_count = 0;

    typedef struct test_case_t
    {
        ds4422_ds4424_t *ds4422_ds4424;
        test_i2c_master_t *i2c_master;
        uint8_t data;

        ds4422_ds4424_error_t expected_ret;
        ds4422_ds4424_i2c_slave_address_t expected_slave_address;
        size_t expected_size;
        uint8_t *expected_data;
    } test_case_t;

    ds4422_ds4424_t ds4422_ds4424[24];
    test_i2c_master_t i2c_master[24];
    for (size_t i = 0; i < 24; i++)
    {
        test_i2c_master_init(&i2c_master[i]);
        ds4422_ds4424_error_t err = ds4422_ds4424_init(&ds4422_ds4424[i], (ds4422_ds4424_i2c_master_t *)&i2c_master[i], (0 <= i && i < 6)     ? DS4422_DS4424_A0_GND_A1_GND //
                                                                                                                        : (6 <= i && i < 12)  ? DS4422_DS4424_A0_VCC_A1_GND //
                                                                                                                        : (12 <= i && i < 18) ? DS4422_DS4424_A0_GND_A1_VCC //
                                                                                                                                              : DS4422_DS4424_A0_VCC_A1_VCC);
        assert(err == DS4422_DS4424_SUCCESS);
    }

    test_case_t test_cases[] = {{.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[0], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[1], /* */ .i2c_master = &i2c_master[1], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xFB, 0x80}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[2], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[3], /* */ .i2c_master = &i2c_master[3], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xFB, 0xFF}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[4], /* */ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[5], /* */ .i2c_master = &i2c_master[5], /* */ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[6], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[7], /* */ .i2c_master = &i2c_master[7], /* */ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xFB, 0x80}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[8], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[9], /* */ .i2c_master = &i2c_master[9], /* */ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_GND, .expected_size = 2, .expected_data = (uint8_t[]){0xFB, 0xFF}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[10], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[11], /**/ .i2c_master = &i2c_master[11], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[12], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[13], /**/ .i2c_master = &i2c_master[13], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xFB, 0x80}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[14], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[15], /**/ .i2c_master = &i2c_master[15], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_GND_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xFB, 0xFF}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[16], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[17], /**/ .i2c_master = &i2c_master[17], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[18], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[19], /**/ .i2c_master = &i2c_master[19], /**/ .data = 0, /*  */ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xFB, 0x80}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[20], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[21], /**/ .i2c_master = &i2c_master[21], /**/ .data = 127, /**/ .expected_ret = DS4422_DS4424_SUCCESS, .expected_slave_address = DS4422_DS4424_A0_VCC_A1_VCC, .expected_size = 2, .expected_data = (uint8_t[]){0xFB, 0xFF}},
                                {.ds4422_ds4424 = NULL, /*              */ .i2c_master = &i2c_master[22], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_NULL_POINTER},
                                {.ds4422_ds4424 = &ds4422_ds4424[23], /**/ .i2c_master = &i2c_master[23], /**/ .data = 128, /**/ .expected_ret = DS4422_DS4424_ERROR_INVALID_DATA}};
    size_t test_size = sizeof(test_cases) / sizeof(test_case_t);

    for (size_t i = 0; i < test_size; i++)
    {
        test_case_t *test_case = &test_cases[i];

        ds4422_ds4424_error_t actual_ret = ds4422_ds4424_out3_source(test_case->ds4422_ds4424, test_case->data);
        if (test_case->expected_ret != actual_ret)
        {
            fprintf(stderr, "%sindex: %d, expected_ret: %d, actual_ret: %d%s\n", TEST_TEXT_RED, i, test_case->expected_ret, actual_ret, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }
        else if (actual_ret != 0)
        {
            continue;
        }

        ds4422_ds4424_i2c_slave_address_t actual_slave_address = test_case->i2c_master->last_written_address;
        if (test_case->expected_slave_address != actual_slave_address)
        {
            fprintf(stderr, "%sindex: %d, expected_slave_address: %d, actual_slave_address: %d%s\n", TEST_TEXT_RED, i, test_case->expected_slave_address, actual_slave_address, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }

        size_t actual_size = test_case->i2c_master->recent_size;
        if (test_case->expected_size != actual_size)
        {
            fprintf(stderr, "%sindex: %d, expected_size: %d, actual_size: %d%s\n", TEST_TEXT_RED, i, test_case->expected_size, actual_size, TEST_TEXT_RESET);
            test_failure_count++;
            continue;
        }

        uint8_t *actual_data = test_case->i2c_master->recent_data;
        for (size_t j = 0; j < actual_size; j++)
        {
            if (test_case->expected_data[j] != actual_data[j])
            {
                fprintf(stderr, "%sindex: %d, expected_data[%d]: %d, actual_data[%d]: %d%s\n", TEST_TEXT_RED, i, j, test_case->expected_data[j], j, actual_data[j], TEST_TEXT_RESET);
                test_failure_count++;
                continue;
            }
        }
    }

    return test_failure_count;
}

int main(void)
{
    int test_failure_count = 0;

    test_failure_count += test_ds4422_ds4424_init();
    test_failure_count += test_ds4422_ds4424_out0_sink();
    test_failure_count += test_ds4422_ds4424_out0_source();
    test_failure_count += test_ds4422_ds4424_out1_sink();
    test_failure_count += test_ds4422_ds4424_out1_source();
    test_failure_count += test_ds4422_ds4424_out2_sink();
    test_failure_count += test_ds4422_ds4424_out2_source();
    test_failure_count += test_ds4422_ds4424_out3_sink();
    test_failure_count += test_ds4422_ds4424_out3_source();

    if (test_failure_count == 0)
    {
        printf("%sOK%s\n", TEST_TEXT_GREEN, TEST_TEXT_RESET);
        return 0;
    }
    else
    {
        fprintf(stderr, "%s%d errors found%s\n", TEST_TEXT_RED, test_failure_count, TEST_TEXT_RESET);
        return 1;
    }
}