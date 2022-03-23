
#include <stdio.h>
#include <stdlib.h>
#include "bit_array.h"

//-----------------------------------------

static const int NUM_ARGC = 1;

//--------

void all_coverage_lib ();

void test_bit_array_construct ();
void test_bit_array_length    ();
void test_bit_array_dump      ();

void test_bit_array_foreach ();
int  test_foreach_func      (uint8_t, void*);

void test_bit_1_operations      ();
void test_bit_region_operations ();

//-----------------------------------------

int main (int argc, const char* argv[])
{
    if (argc != NUM_ARGC)
    {
        fprintf (stderr, "ERROR! Usage ./bit_array_test\n");
        exit    (EXIT_FAILURE);
    }

    all_coverage_lib ();
}

//--------

void all_coverage_lib ()
{
    test_bit_array_construct   ();
    test_bit_array_length      ();
    test_bit_array_dump        ();
    test_bit_array_foreach     ();
    test_bit_1_operations      ();
    test_bit_region_operations ();
    test_bit_array_foreach     ();
}

void test_bit_array_construct ()
{
    my_bit_array* ba1 = bit_array_construct (    -1);
    my_bit_array* ba2 = bit_array_construct (     1);
    my_bit_array* ba3 = bit_array_construct (282828);
    my_bit_array* ba4 = bit_array_construct (  2828);
    my_bit_array* ba5 = bit_array_construct (    28);

    bit_array_destruct (ba5);
    bit_array_destruct (ba4);
    bit_array_destruct (ba3);
    bit_array_destruct (ba2);
    bit_array_destruct (ba1);
}

//----

void test_bit_array_length ()
{
    for (size_t i = 0; i < 4; i++)
    {
        my_bit_array* ba1 = bit_array_construct (282828);
        bit_array_length   (ba1);
        bit_array_destruct (ba1);
    }
}

//----

void test_bit_array_dump ()
{
    for (size_t i = 0; i < 10; i++)
    {
        my_bit_array* ba = bit_array_construct (28);
        bit_array_dump     (ba);
        bit_array_destruct (ba);
    }
}

//----

void test_bit_array_foreach ()
{
    for (size_t i = 0; i < 5; i++)
    {
        int (*func) (uint8_t, void*) = test_foreach_func;
        
        if (i == 0 || i == 1)
            func = NULL;

        my_bit_array* ba = bit_array_construct (1);
        bit_array_foreach  (ba, func, NULL);
        bit_array_destruct (ba);
    }
}

//-

int test_foreach_func (uint8_t bit, void* data)
{
    bit;
    data;
    return 0;
}

//----

void test_bit_1_operations ()
{
    for (size_t i = 0; i < 4; i++)
    {
        my_bit_array* ba = bit_array_construct (28);

        bit_array_set_bit    (ba, 16);
        bit_array_clear_bit  (ba, 16);
        bit_array_get_bit    (ba, 16);
        bit_array_toggle_bit (ba, 16);

        bit_array_set_bit    (ba, 29);
        bit_array_clear_bit  (ba, 29);
        bit_array_get_bit    (ba, 29);
        bit_array_toggle_bit (ba, 29);

        bit_array_destruct (ba);
    }
}

//----

void test_bit_region_operations ()
{
    for (size_t i = 0; i < 4; i++)
    {
        my_bit_array* ba = bit_array_construct (150);
        
        bit_array_set_region    (ba, 1, 7);
        bit_array_clear_region  (ba, 1, 7);
        bit_array_toggle_region (ba, 1, 7);

        bit_array_set_region    (ba, 1, 0);
        bit_array_clear_region  (ba, 1, 0);
        bit_array_toggle_region (ba, 1, 0);

        bit_array_set_region    (ba, 60, 80);
        bit_array_clear_region  (ba, 60, 80);
        bit_array_toggle_region (ba, 60, 80);
        
        bit_array_destruct (ba);
    }
}


/*
TODO
3) find
4) test to find
5) dynamic array (double up)
*/

//-----------------------------------------