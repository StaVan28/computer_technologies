
#ifndef BIT_ARRAY_H_INCLUDED
#define BIT_ARRAY_H_INCLUDED

//-----------------------------------------

#define DEBUG

#include <stdint.h>

//-----------------------------------------

struct my_bit_array
{
    uint64_t num_of_bits;
    uint64_t num_of_words;

    uint64_t* arr_bits;
    uint64_t  capacity;
} my_bit_array;

//-----------------------------------------

void test_func (int a);

//-----------------------------------------

#endif // BIT_ARRAY_H_INCLUDED