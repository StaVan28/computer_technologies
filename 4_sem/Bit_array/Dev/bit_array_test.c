
#include <stdio.h>
#include <stdlib.h>
#include "bit_array.h"

//-----------------------------------------

static const int NUM_ARGC = 1;

//-----------------------------------------

int main (int argc, const char* argv[])
{
    if (argc != NUM_ARGC)
    {
        fprintf (stderr, "ERROR! Usage ./bit_array_test\n");
        exit    (EXIT_FAILURE);
    }

    my_bit_array bit_array;
    bit_array_init   (&bit_array, 7);
    bit_array_deinit (&bit_array);

    my_bit_array* bit_array2 = bit_array_construct (7);
    bit_array_destruct (bit_array2);
}

//-----------------------------------------