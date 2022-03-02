
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

    my_bit_array bit_array2;
    bit_array_init   (&bit_array2, 7);
    bit_array_deinit (&bit_array2);

    my_bit_array* bit_array = bit_array_construct (7);

    bit_array_set_bit   (bit_array, 3);
    bit_array_set_bit   (bit_array, 2);
    bit_array_clear_bit (bit_array, 2);
    printf ("%d\n", bit_array_get_bit (bit_array, 3));
    bit_array_toggle_bit (bit_array, 6);
    
    bit_array_dump (bit_array);
    bit_array_destruct (bit_array);

}

//-----------------------------------------