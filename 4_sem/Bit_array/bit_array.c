
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "bit_array.h"

/*
typedef struct my_bit_array
{
    bit_index_t num_of_bits_;
    word_t      num_of_words_;

    word_t  capacity_;
    word_t* buff_bits_;
} my_bit_array;
*/

//-----------------------------------------

#define ROUNDUP_BITS_TO_WORDS(bits) (((bits) + 63) / 64)
#define MIN_CAPACITY                2

//-----

static word_t roundup_word64_to_pow2 (word_t word64)
{
    word64--;
    word64 |= word64 >> 1;
    word64 |= word64 >> 2;
    word64 |= word64 >> 4;
    word64 |= word64 >> 8;
    word64 |= word64 >> 16;
    word64 |= word64 >> 32;
    word64++;

    word64 += (word64 == 0);
    
    return word64;
}


//-----------------------------------------

my_bit_array* bit_array_init (my_bit_array* bit_array, bit_index_t num_of_bits)
{
    bit_array->num_of_bits_  = num_of_bits;
    bit_array->num_of_words_ = ROUNDUP_BITS_TO_WORDS(num_of_bits);

    word_t word64_to_pow2 = roundup_word64_to_pow2 (bit_array->num_of_words_);
    if (word64_to_pow2 > MIN_CAPACITY)
        bit_array->capacity_ = word64_to_pow2;
    else
        bit_array->capacity_ = MIN_CAPACITY;

    bit_array->buff_bits_ = (word_t*) calloc (bit_array->capacity_, sizeof (word_t));
    if (bit_array->buff_bits_ == NULL)
    {
        errno = ENOMEM;
        return NULL;
    }

    return bit_array;
}

//--------

void bit_array_deinit (my_bit_array* bit_array)
{
    free (bit_array->buff_bits_);
    bit_array = NULL;
}

//--------

my_bit_array* bit_array_construct (bit_index_t num_of_bits)
{
    my_bit_array* bit_array = (my_bit_array*) calloc (1, sizeof (my_bit_array));
    if (!bit_array)
    {
        errno = ENOMEM;
        return NULL;
    }

    my_bit_array* ret_init = bit_array_init (bit_array, num_of_bits);
    if (!ret_init)
    {
        errno = ENOMEM;
        return NULL;
    }

    return bit_array;
}

//--------

void bit_array_destruct (my_bit_array* bit_array)
{
    if (bit_array->buff_bits_ != NULL)
        bit_array_deinit (bit_array);

    free (bit_array);
    bit_array = NULL;
}


//-----------------------------------------