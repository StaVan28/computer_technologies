
#ifndef BIT_ARRAY_H_INCLUDED
#define BIT_ARRAY_H_INCLUDED

//-----------------------------------------

#include <stdint.h>

//-----------------------------------------

typedef uint64_t bit_index_t, word_t;

//-----------------------------------------

typedef struct my_bit_array
{
    bit_index_t num_of_bits_;
    word_t      num_of_words_;

    word_t  capacity_;
    word_t* buff_bits_;
} my_bit_array;

//-----------------------------------------

my_bit_array* bit_array_construct (bit_index_t num_of_bits);
void          bit_array_destruct  (my_bit_array* bit_array);

my_bit_array* bit_array_init   (my_bit_array* bit_array, bit_index_t num_of_bits);
void          bit_array_deinit (my_bit_array* bit_array);

//my_bit_array* bit_array_alloc   (my_bit_array* bit_array);
//my_bit_array* bit_array_dealloc (my_bit_array* bit_array);

//-----------------------------------------

#endif // BIT_ARRAY_H_INCLUDED