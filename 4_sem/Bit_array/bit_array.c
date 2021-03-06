
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include "bit_array.h"

//-----------------------------------------

//-------------------
//    define, consts
//-------------------

#define WORD_SIZE           64
#define MIN_CAPACITY        2

//-----

static const char* DUMP_FILE_BIT_ARRAY = "dump_bit_array.txt";

//-------------------
//    tests funcs
//-------------------

#ifdef _RELEASE
    #define MY_MALLOC(size)         malloc (size)
    #define MY_CALLOC(nmemb, size)  calloc (nmemb, size)
    #define MY_FOPEN(path, flags)   fopen  (path, flags)
    #define ERROR_INFO(err_str)                                     \
                do {                                                \
                    perror ("ERROR! " err_str);                     \
                } while (0)
#else
    #define MY_MALLOC(size)         test_malloc (size)
    #define MY_CALLOC(nmemb, size)  test_calloc (nmemb, size)
    #define MY_FOPEN(path, flags)   test_fopen  (path, flags)
    #define ERROR_INFO(err_str)                                      \
                do {                                                 \
                                                                     \
                } while (0)
#endif

//-----

static size_t GL_COUNTER_ALLOC = 0;

//-

static void* test_malloc(size_t size)
{
    GL_COUNTER_ALLOC++;
    
    if (GL_COUNTER_ALLOC % 3)
        return malloc (size);

    else
        return NULL;
}

//-----

static void* test_calloc(size_t nmemb, size_t size)
{
    GL_COUNTER_ALLOC++;

    if (GL_COUNTER_ALLOC % 4)
        return calloc (nmemb, size);
    else
        return NULL;
}

//-----

static size_t GL_COUNTER_FOPEN = 0;

//-

static FILE* test_fopen(const char* pathname, const char* mode)
{
    GL_COUNTER_FOPEN++;

    if (GL_COUNTER_FOPEN % 3)
        return fopen(pathname, mode);
    else
        return NULL;
}

//-------------------
//    bit opers
//-------------------

#define WORD_MAX    ~(word_t) 0
#define WORD_MIN     (word_t) 0

#define ROUNDUP_BITS_TO_WORDS(bits) (((bits) + WORD_SIZE - 1) / WORD_SIZE)
#define NUMB_OF_WORD64(pos)         ((pos) >> 6)
#define INDX_IN_WORD64(pos)         ((pos) & (WORD_SIZE - 1))

//-----

static word_t bit_mask_64 (bit_index_t length)
{
    if (length)
        return WORD_MAX << ( WORD_SIZE - (length) );
    else
        return WORD_MIN;
}

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

//-------------------
//    resize
//-------------------

static char bit_array_resize (my_bit_array* bit_array, bit_index_t new_num_of_bits)
{
    word_t old_num_of_words = bit_array->num_of_words;
    word_t new_num_of_words = ROUNDUP_BITS_TO_WORDS (new_num_of_bits);

    bit_array->num_of_bits  = new_num_of_bits;
    bit_array->num_of_words = new_num_of_words;

    if (new_num_of_words > bit_array->capacity)
    {
        word_t old_capacity_in_words = bit_array->capacity;
        word_t old_capacity_in_bytes = bit_array->capacity * WORD_SIZE;

        bit_array->capacity          = roundup_word64_to_pow2 (bit_array->num_of_words);
        word_t new_capacity_in_bytes = bit_array->capacity * WORD_SIZE;

        word_t* tmp_buff_bits = bit_array->buff_bits;

        bit_array->buff_bits = (word_t*) realloc (bit_array->buff_bits, bit_array->capacity * WORD_SIZE);
        if (bit_array->buff_bits == NULL)
        {
            bit_array->buff_bits = tmp_buff_bits;
            ERROR_INFO ("realloc");

            return 1;
        }

        bit_index_t diff_capacity_of_bits = new_capacity_in_bytes - old_capacity_in_bytes;
        memset (bit_array->buff_bits + old_capacity_in_words, 0, diff_capacity_of_bits);
    }
    else if(new_num_of_words < old_num_of_words)
    {
        bit_index_t diff_capacity_of_bits = (old_num_of_words - new_num_of_words) * sizeof(bit_index_t);
        memset (bit_array->buff_bits + new_num_of_words, 0, diff_capacity_of_bits);
    }

    return 0;
}

//-------------------
//    constr, init
//-------------------

void bit_array_init (my_bit_array* bit_array, bit_index_t num_of_bits)
{
    bit_array->num_of_bits  = num_of_bits;

    bit_array->num_of_words = ROUNDUP_BITS_TO_WORDS  (num_of_bits);
    word_t word64_to_pow2   = roundup_word64_to_pow2 (bit_array->num_of_words);
    if (word64_to_pow2 > MIN_CAPACITY)
        bit_array->capacity = word64_to_pow2;
    else
        bit_array->capacity = MIN_CAPACITY;

    bit_array->buff_bits = (word_t*) MY_CALLOC(bit_array->capacity, sizeof (word_t));
    if (bit_array->buff_bits == NULL)
    {
        errno = ENOMEM;
        ERROR_INFO ("calloc");
        return;
    }
}

//--------

void bit_array_deinit (my_bit_array* bit_array)
{
    free (bit_array->buff_bits);
}

//--------

my_bit_array* bit_array_construct (bit_index_t num_of_bits)
{
    my_bit_array* bit_array = (my_bit_array*) MY_MALLOC(sizeof (my_bit_array));
    if (bit_array == NULL)
    {
        ERROR_INFO ("create struct");
        errno = ENOMEM;
        return NULL;
    }

    errno = 0;
    bit_array_init (bit_array, num_of_bits);
    if (errno)
    {
        ERROR_INFO ("init struct");
        free (bit_array);
        return NULL;
    }

    return bit_array;
}

//--------

void bit_array_destruct (my_bit_array* bit_array)
{
    if (bit_array == NULL)
    {
        ERROR_INFO ("null struct");
        return;
    }

    bit_array_deinit (bit_array);

    free (bit_array);
}

//----------------
// length, dump, foreach
//----------------

bit_index_t bit_array_length (my_bit_array* bit_array)
{
    if (bit_array == NULL)
    {
        ERROR_INFO ("null struct");
        return 0;
    }

    return bit_array->num_of_bits;
}

//--------

char bit_array_dump (my_bit_array* bit_array)
{
    if (bit_array == NULL)
    {
        ERROR_INFO ("null struct");
        return EXIT_FAILURE;
    }    

    FILE* dump = MY_FOPEN(DUMP_FILE_BIT_ARRAY, "wb");
    if (dump == NULL)
    {
        ERROR_INFO ("Incorrect open of dump");
        return EXIT_FAILURE;
    }

    fprintf (dump,     "|-------------------------------------|\n"
                       "|           DUMP BIT ARRAY            |\n"
                       "|-------------------------------------|\n"
                       "\n"
                       "    num_of_bits  = [%" PRIu64 "];\n"
                       "    num_of_words = [%" PRIu64 "];\n"
                       "    capacity     = [%" PRIu64 "];\n"
                       "    buff_bits    = [%p];\n"
                       "\n"
                       "    Buffer bits:\n",
                            bit_array->num_of_bits,
                            bit_array->num_of_words,
                            bit_array->capacity,
                            bit_array->buff_bits
                       );

    for (word_t i_word = 0; i_word < bit_array->capacity; i_word++)
    {
        fprintf (dump, "    word[#%" PRIu64 "]", i_word);

        for (int i_bit = 0; i_bit <= WORD_SIZE - 1; i_bit++)
        {
            if      (i_bit % 16 == 0)
                fprintf (dump, "\n    ");
            else if (i_bit % 4  == 0)
                fprintf (dump, " ");

            fprintf (dump, "%ld", (bit_array->buff_bits[i_word] >> i_bit) & 1);
        }

        fprintf (dump, "\n");
    }

    fprintf (dump,     "\n"
                       "|-------------------------------------|\n"
                       "|           END DUMP BIT ARRAY        |\n"
                       "|-------------------------------------|\n"
                       );

    fclose (dump);

    return EXIT_SUCCESS;
}

//--------

long bit_array_foreach (my_bit_array* bit_array, int (*func) (uint8_t, void*, void*), void* data, void* result)
{
    if (bit_array == NULL)
    {
        ERROR_INFO ("null struct");
        return EXIT_FAILURE;
    }

    if (func == NULL)
    {
        ERROR_INFO ("null func");
        return EXIT_FAILURE;
    }

    long ret_func = 0;

    for (bit_index_t i = 0; i < bit_array->num_of_bits; i++)
        ret_func += func (bit_array_get_bit (bit_array, i), data, result);

    return ret_func;
}

//-----------------
// bit operations (1 bit)
//-----------------

void bit_array_set_bit (my_bit_array* bit_array, bit_index_t index)
{
    if (bit_array == NULL)
    {
        ERROR_INFO ("null struct");
        return;
    }

    if (index >= bit_array->num_of_bits)
    {
        bit_array_resize (bit_array, index);
        return;
    }

    bit_array->buff_bits[index / WORD_SIZE] |= (1 << (index % WORD_SIZE));
}

//--------

void bit_array_clear_bit (my_bit_array* bit_array, bit_index_t index)
{
    if (bit_array == NULL)
    {
        ERROR_INFO ("null struct");
        return;
    }

    if (index >= bit_array->num_of_bits)
    {
        bit_array_resize (bit_array, index);
        return;
    }

    bit_array->buff_bits[index / WORD_SIZE] &= ~(1 << (index % WORD_SIZE));
}

//--------

int bit_array_get_bit (my_bit_array* bit_array, bit_index_t index)
{
    if (bit_array == NULL)
    {
        ERROR_INFO ("null struct");
        return 0;
    }

    if (index >= bit_array->num_of_bits)
    {
        bit_array_resize (bit_array, index);
        return 0;
    }

    return ((bit_array->buff_bits[index / WORD_SIZE] & (1 << (index % WORD_SIZE))) != 0);  
}

//--------

void bit_array_toggle_bit (my_bit_array* bit_array, bit_index_t index)
{
    if (bit_array == NULL)
    {
        ERROR_INFO ("null struct");
        return;
    }

    if (index >= bit_array->num_of_bits)
    {
        bit_array_resize (bit_array, index);
        return;
    }

    bit_array->buff_bits[index / WORD_SIZE] ^= (1 << (index % WORD_SIZE));
}

//--------------
//   bit operation (region)
//--------------

void bit_array_set_region (my_bit_array* bit_array, bit_index_t start, bit_index_t length)
{
    if (bit_array == NULL)
    {
        ERROR_INFO ("null struct");
        return;
    }

    if (length == 0)
        return;

    word_t  first_word   = NUMB_OF_WORD64 (start);
    uint8_t first_offset = INDX_IN_WORD64 (start);

    word_t  last_word   = NUMB_OF_WORD64 (start + length - 1);
    uint8_t last_offset = INDX_IN_WORD64 (start + length - 1);

    if (first_word == last_word)
    {
        word_t mask = bit_mask_64 (length) << first_offset;
        bit_array->buff_bits[first_word] |= mask;
    }
    else
    {
        // first word
        bit_array->buff_bits[first_word] |= ~bit_mask_64 (first_offset);

        for (word_t i_word = first_word + 1; i_word < last_word; i_word++)
            bit_array->buff_bits[i_word] = WORD_MAX;

        bit_array->buff_bits[last_word]  |=  bit_mask_64 (last_offset + 1);
    }
}

//-------

void bit_array_clear_region (my_bit_array* bit_array, bit_index_t start, bit_index_t length)
{
    if (bit_array == NULL)
    {
        ERROR_INFO ("null struct");
        return;
    }

    if (length == 0)
        return;

    word_t  first_word   = NUMB_OF_WORD64 (start);
    uint8_t first_offset = INDX_IN_WORD64 (start);

    word_t  last_word   = NUMB_OF_WORD64 (start + length - 1);
    uint8_t last_offset = INDX_IN_WORD64 (start + length - 1);

    if (first_word == last_word)
    {
        word_t mask = bit_mask_64 (length) << first_offset;
        bit_array->buff_bits[first_word] &= ~mask;
    }
    else
    {
        // first word
        bit_array->buff_bits[first_word] &=  bit_mask_64 (first_offset);

        for (word_t i_word = first_word + 1; i_word < last_word; i_word++)
            bit_array->buff_bits[i_word] = WORD_MIN;

        bit_array->buff_bits[last_word]  &= ~bit_mask_64 (last_offset + 1);
    }
}

//-------

void bit_array_toggle_region (my_bit_array* bit_array, bit_index_t start, bit_index_t length)
{
    if (bit_array == NULL)
    {
        ERROR_INFO ("null struct");
        return;
    }

    if (length == 0)
        return;

    word_t  first_word   = NUMB_OF_WORD64 (start);
    uint8_t first_offset = INDX_IN_WORD64 (start);

    word_t  last_word    = NUMB_OF_WORD64 (start + length - 1);
    uint8_t last_offset  = INDX_IN_WORD64 (start + length - 1);

    if (first_word == last_word)
    {
        word_t mask = bit_mask_64 (length) << first_offset;
        bit_array->buff_bits[first_word] ^= mask;
    }
    else
    {
        // first word
        bit_array->buff_bits[first_word] ^= ~bit_mask_64 (first_offset);

        for (word_t i_word = first_word + 1; i_word < last_word; i_word++)
            bit_array->buff_bits[i_word] ^= WORD_MAX;

        bit_array->buff_bits[last_word]  ^=  bit_mask_64 (last_offset + 1);
    }
}

//--------------
//   find funcs
//--------------

// return 0 if no bits = 1
char bit_array_find_set_bit (my_bit_array* bit_array, bit_index_t start, bit_index_t* result)
{
    if (bit_array == NULL)
    {
        ERROR_INFO ("null struct");
        return 0;
    }

    if (start > bit_array->num_of_bits)
        return 0;

    word_t   i_word = NUMB_OF_WORD64 (start);
    word_t cur_word = ( bit_array->buff_bits[i_word]) & ~(bit_mask_64 (INDX_IN_WORD64 (start)) );

    while (1) 
    {
        if (cur_word > 0)
        {
            bit_index_t pos = i_word * WORD_SIZE + __builtin_ffsll (cur_word) - 1;
            if (pos < bit_array->num_of_bits) 
            { 
                *result = pos; 
                return 1;
            }
        }
        
        i_word++;
        
        if(i_word >= bit_array->num_of_words)
            break;

        cur_word = bit_array->buff_bits[i_word];
    }

    return 0;
}

//-------

char bit_array_find_clear_bit (my_bit_array* bit_array, bit_index_t start, bit_index_t* result)
{
    if (bit_array == NULL)
    {
        ERROR_INFO ("null struct");
        return 0;
    }

    if (start > bit_array->num_of_bits)
        return 0;

    word_t   i_word = NUMB_OF_WORD64 (start);
    word_t cur_word = ~( (bit_array->buff_bits[i_word]) | (bit_mask_64 (INDX_IN_WORD64 (start))) );

    while (1) 
    {
        if (cur_word > 0)
        {
            bit_index_t pos = i_word * WORD_SIZE + __builtin_ffsll (cur_word) - 1;
            if (pos < bit_array->num_of_bits) 
            { 
                *result = pos; 
                return 1;
            }
        }
        
        i_word++;
        
        if(i_word >= bit_array->num_of_words)
            break;

        cur_word = ~bit_array->buff_bits[i_word];
    }

    return 0;
}

//-----------------------------------------