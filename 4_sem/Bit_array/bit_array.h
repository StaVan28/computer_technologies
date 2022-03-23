
#ifndef BIT_ARRAY_H_INCLUDED
#define BIT_ARRAY_H_INCLUDED

//-----------------------------------------

#include <stdint.h>

//-----------------------------------------

typedef uint64_t bit_index_t, word_t;

//-----------------------------------------

typedef struct my_bit_array
{
    bit_index_t num_of_bits;

    word_t  num_of_words;
    word_t  capacity;
    word_t* buff_bits;
} my_bit_array;

//-----------------------------------------

my_bit_array* bit_array_construct (bit_index_t num_of_bits);
void          bit_array_destruct  (my_bit_array* bit_array);

void bit_array_init   (my_bit_array* bit_array, bit_index_t num_of_bits);
void bit_array_deinit (my_bit_array* bit_array);

void        bit_array_dump   (my_bit_array* bit_array);
bit_index_t bit_array_length (my_bit_array* bit_array);

void bit_array_set_bit    (my_bit_array* bit_array, bit_index_t index);
void bit_array_clear_bit  (my_bit_array* bit_array, bit_index_t index);
int  bit_array_get_bit    (my_bit_array* bit_array, bit_index_t index);
void bit_array_toggle_bit (my_bit_array* bit_array, bit_index_t index);

void bit_array_set_region    (my_bit_array* bit_array, bit_index_t start, bit_index_t length);
void bit_array_clear_region  (my_bit_array* bit_array, bit_index_t start, bit_index_t length);
void bit_array_toggle_region (my_bit_array* bit_array, bit_index_t start, bit_index_t length);

//char bit_array_find_first_set_bit   (my_bit_array* bit_array);
//char bit_array_find_first_clear_bit (my_bit_array* bit_array);

//char bit_array_find_next_set_bit   (my_bit_array* bit_array, bit_index_t start, bit_index_t* result);
//char bit_array_find_next_clear_bit (my_bit_array* bit_array, bit_index_t start, bit_index_t* result);

void bit_array_foreach (my_bit_array* bit_array, int (*func) (uint8_t, void*), void* data);

//-----------------------------------------

#endif // BIT_ARRAY_H_INCLUDED