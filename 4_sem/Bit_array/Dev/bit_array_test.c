
#include <stdio.h>
#include <stdlib.h>
#include "bit_array.h"

//-----------------------------------------

//-------------------------
//    Tests Init
//-------------------------

const char* SUITE_NAME = 0;
static char SUITE_PASS = 0;

static int SUITES_RUN    = 0;
static int SUITES_FAILED = 0;
static int SUITES_EMPTY  = 0;

static int TESTS_IN_SUITE = 0;
static int TESTS_RUN      = 0;
static int TESTS_FAILED   = 0;

//--------

#define ASSERT(x)                                                                           \
         do {                                                                               \
            TESTS_RUN++;                                                                    \
            TESTS_IN_SUITE++;                                                               \
            if(!(x))                                                                        \
            {                                                                               \
                fprintf(stderr, "failed assert [%s:%i] %s\n", __FILE__, __LINE__, #x);      \
                SUITE_PASS = 0;                                                             \
                TESTS_FAILED++;                                                             \
            }                                                                               \
        } while (0)

//--------

void suite_start (const char *name)
{
    SUITE_PASS     = 1;
    SUITE_NAME     = name;
    TESTS_IN_SUITE = 0;

    SUITES_RUN++;
}

void suite_end ()
{
    printf ("Testing %12.12s  ...................... ", SUITE_NAME);
    if (SUITE_PASS)
        printf ("pass\n");
    else
        printf ("fail\n");
    
    if (SUITE_PASS == 0)
        SUITES_FAILED++;

    if (TESTS_IN_SUITE == 0)
        SUITES_EMPTY++;
}

//------------

static const int NUM_ARGC = 1;

//--------

void all_coverage_lib ();

void test_bit_array_construct ();
void test_bit_array_length    ();
void test_bit_array_dump      ();

void test_bit_array_foreach ();
int  test_foreach_func      (uint8_t, void*, void*);

void test_bit_1_operations      ();
void test_bit_region_operations ();

void test_bit_array_find_bit ();

static char cmp_arrays (my_bit_array* ba, bit_index_t start, bit_index_t length);

//-----------------------------------------

int main (int argc, const char* argv[])
{
    if (argc != NUM_ARGC)
    {
        fprintf (stderr, "ERROR! Usage ./bit_array_test\n");
        exit    (EXIT_FAILURE);
    }

    printf ("        TEST bit array lib:\n");


    all_coverage_lib ();


    printf("\n");
    printf(" %i / %i suites failed\n", SUITES_FAILED, SUITES_RUN);
    printf(" %i / %i suites empty\n" , SUITES_EMPTY , SUITES_RUN);
    printf(" %i / %i tests failed\n" , TESTS_FAILED ,   TESTS_RUN);

    printf("\n        THE END.\n");

    return TESTS_FAILED ? EXIT_FAILURE : EXIT_SUCCESS;
}

//--------

void all_coverage_lib ()
{
    test_bit_array_find_bit    ();
    test_bit_array_construct   ();
    test_bit_array_length      ();
    test_bit_array_dump        ();
    test_bit_array_foreach     ();
    test_bit_1_operations      ();
    test_bit_region_operations ();
}

void test_bit_array_construct ()
{
    suite_start ("constr/destr");

    my_bit_array* ba1 = bit_array_construct (    -1);
    my_bit_array* ba2 = bit_array_construct (     1);
    my_bit_array* ba3 = bit_array_construct (282828);
    my_bit_array* ba4 = bit_array_construct (  2828);
    my_bit_array* ba5 = bit_array_construct (    28);

    ASSERT (ba3);

    bit_array_destruct (ba5);
    bit_array_destruct (ba4);
    bit_array_destruct (ba3);
    bit_array_destruct (ba2);
    bit_array_destruct (ba1);

    suite_end ();
}

//----

void test_bit_array_length ()
{
    suite_start ("length");

    int size = 282828;

    for (size_t i = 0; i < 4; i++)
    {
        my_bit_array* ba1 = bit_array_construct (size);
        
        bit_index_t ret_length = bit_array_length (ba1);
        if (ba1 != NULL)
            ASSERT (ret_length == size);
        
        bit_array_destruct (ba1);
    }

    suite_end ();
}

//----

void test_bit_array_dump ()
{
    suite_start ("dump");

    int size              = 28;
    int call_dump_counter = 0;

    for (size_t i = 0; i < 10; i++)
    {
        call_dump_counter++;

        my_bit_array* ba = bit_array_construct (size);
        size += 28;

        if (ba != NULL || !(call_dump_counter % 3))
            bit_array_dump (ba);
        else
            ASSERT (bit_array_dump (ba));

        bit_array_destruct (ba);
    }

    suite_end (); 
}

//----

void test_bit_array_foreach ()
{
    suite_start ("foreach");

    for (size_t i = 0; i < 10; i++)
    {
        int (*func) (uint8_t, void*, void*) = test_foreach_func;
        
        if (i == 0 || i == 1)
            func = NULL;

        my_bit_array* ba = bit_array_construct (1);

        if (ba != NULL || func != NULL)
            bit_array_foreach (ba, func, NULL, NULL);
        else
            ASSERT (bit_array_foreach (ba, func, NULL, NULL));
        
        bit_array_destruct (ba);
    }

    suite_end ();
}

//-

int test_foreach_func (uint8_t bit, void* data, void* result)
{
    bit;
    data;
    result;
    return 0;
}

//----

void test_bit_1_operations ()
{
    suite_start ("1 bit oper");

    for (size_t i = 0; i < 8; i++)
    {
        my_bit_array* ba = bit_array_construct (28);

        bit_array_set_bit (ba, 16);
        if (ba == NULL)
            bit_array_get_bit (ba, 16);
        else
            ASSERT (bit_array_get_bit (ba, 16) == 1);

        bit_array_clear_bit (ba, 16);
        if (ba == NULL)
            bit_array_get_bit (ba, 16);
        else
            ASSERT (bit_array_get_bit (ba, 16) == 0);

        bit_array_toggle_bit (ba, 16);
        if (ba == NULL)
            bit_array_get_bit (ba, 16);
        else
            ASSERT (bit_array_get_bit (ba, 16) == 1);

        bit_array_toggle_bit (ba, 16);
        if (ba == NULL)
            bit_array_get_bit (ba, 16);
        else
            ASSERT (bit_array_get_bit (ba, 16) == 0);

        // covering the boundary case
        bit_array_set_bit    (ba, 200);
        bit_array_clear_bit  (ba, 200);
        bit_array_get_bit    (ba, 200);
        bit_array_toggle_bit (ba, 200);

        bit_array_destruct (ba);
    }

    suite_end ();
}

//----

void test_bit_region_operations ()
{
    suite_start ("region oper");

    for (size_t i = 0; i < 4; i++)
    {
        my_bit_array* ba = bit_array_construct (150);
        
        bit_array_set_region (ba, 1, 7);
        ASSERT (cmp_arrays (ba, 1, 7));

        bit_array_clear_region (ba, 1, 7);
        ASSERT (cmp_arrays (ba, 1, 7));

        bit_array_toggle_region (ba, 1, 7);
        ASSERT (cmp_arrays (ba, 1, 7));

        bit_array_set_region    (ba, 1, 0);
        ASSERT (cmp_arrays (ba, 1, 7));

        bit_array_clear_region  (ba, 1, 0);
        ASSERT (cmp_arrays (ba, 1, 7));

        bit_array_toggle_region (ba, 1, 0);
        ASSERT (cmp_arrays (ba, 1, 7));

        bit_array_set_region    (ba, 60, 80);
        ASSERT (cmp_arrays (ba, 1, 7));

        bit_array_clear_region  (ba, 60, 80);
        ASSERT (cmp_arrays (ba, 1, 7));

        bit_array_toggle_region (ba, 60, 80);
        ASSERT (cmp_arrays (ba, 1, 7));
        
        bit_array_destruct (ba);
    }

    suite_end ();
}

//-----------------------------------------

void test_bit_array_find_bit ()
{
    suite_start ("find bit");

    for (size_t i = 0; i < 8; i++)
    {
        my_bit_array* ba = bit_array_construct (150);

        bit_index_t result = 0;
        bit_index_t ret    = bit_array_find_set_bit (ba, 0, &result);
        ASSERT (ret == 0);

        bit_array_set_bit (ba, 3);
        result = 0;
        ret    = bit_array_find_set_bit (ba, 0, &result);
        if (ret == 1)
            ASSERT (result == 3);

        result = 0;
        ret    = bit_array_find_set_bit (ba, 151, &result);
        ASSERT (ret == 0);

        bit_array_destruct (ba);        
    }

    for (size_t i = 0; i < 8; i++)
    {
        my_bit_array* ba = bit_array_construct (150);

        bit_index_t result = 0;
        bit_index_t ret    = bit_array_find_clear_bit (ba, 0, &result);
        if (ret == 1)
            ASSERT (result == 0);

        for (size_t i = 0; i < 3; i++)
            bit_array_set_bit (ba, i);

        result = 0;
        ret    = bit_array_find_clear_bit (ba, 0, &result);
        if (ret == 1)
            ASSERT (result == 3);

        for (size_t i = 0; i < 90; i++)
            bit_array_set_bit (ba, i);

        result = 0;
        ret    = bit_array_find_clear_bit (ba, 0, &result);
        if (ret == 1)
            ASSERT (result == 90);

        for (size_t i = 0; i < 150; i++)
            bit_array_set_bit (ba, i);

        result = 0;
        ret    = bit_array_find_clear_bit (ba, 0, &result);
        ASSERT (ret == 0);

        bit_array_destruct (ba);        
    }

    suite_end ();
}

//-----------------------------------------

static char cmp_arrays (my_bit_array* ba, bit_index_t start, bit_index_t length)
{
    if (ba == NULL)
        return EXIT_FAILURE;

    my_bit_array* ba_cmp = bit_array_construct (ba->num_of_bits);
    if (ba_cmp == NULL)
        return EXIT_FAILURE;

    for (bit_index_t i = 0; i < length; i++)
        bit_array_set_bit (ba_cmp, start + i);

    for (bit_index_t i = 0; i < length; i++)
    {
        if (bit_array_get_bit (ba, i) != bit_array_get_bit (ba_cmp, i))
        {
            bit_array_destruct (ba_cmp);
            return EXIT_FAILURE;
        }
    }

    bit_array_destruct (ba);
    return EXIT_SUCCESS;
}