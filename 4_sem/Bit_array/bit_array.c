
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include "bit_array.h"

//-----------------------------------------

//-------------------
//    define, consts
//-------------------

#define WORD_SIZE           64
#define MIN_CAPACITY        2

//-----

const char* DUMP_FILE_BIT_ARRAY = "dump_bit_array.txt";

//-----

#ifdef _RELEASE
    #define MY_MALLOC(size)         malloc (size)
    #define MY_CALLOC(nmemb, size)  calloc (nmemb, size)
    #define MY_FOPEN(path, flags)   fopen  (path, flags)
#else
    #define MY_MALLOC(size)         test_malloc (size)
    #define MY_CALLOC(nmemb, size)  test_calloc (nmemb, size)
    #define MY_FOPEN(path, flags)   test_fopen  (path, flags)
#endif

//-----

size_t GL_COUNTER_ALLOC = 0;

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

size_t GL_COUNTER_FOPEN = 0;

//-

FILE* test_fopen(const char* pathname, const char* mode)
{
    GL_COUNTER_FOPEN++;

    if (GL_COUNTER_FOPEN % 3)
        return fopen(pathname, mode);
    else
        return NULL;
}

//-----

#define ERROR_INFO(err_str)                                      \
            do {                                                 \
                perror  (        "ERROR! " err_str);             \
                fprintf (stderr, "errno = %d\n", errno);         \
                                                                 \
                fprintf (stderr, "file = {%s}, line = {%d}\n",   \
                                  __FILE__,    __LINE__       ); \
            } while (0);                                         \

//-----

#define WORD_MAX    ~(word_t) 0
#define WORD_MIN     (word_t) 0

#define ROUNDUP_BITS_TO_WORDS(bits) (((bits) + WORD_SIZE - 1) / WORD_SIZE)
#define NUMB_OF_WORD64(pos)         ((pos) >> 6)
#define INDX_IN_WORD64(pos)         ((pos) & (WORD_SIZE - 1))

//-----

static word_t bit_mask_64 (bit_index_t length)
{
    if (length)
        return WORD_MAX << (WORD_SIZE - (length));
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
//    constr, init
//-------------------

void bit_array_init (my_bit_array* bit_array, bit_index_t num_of_bits)
{
    bit_array->num_of_bits  = num_of_bits;

    word_t word64_to_pow2 = roundup_word64_to_pow2 (ROUNDUP_BITS_TO_WORDS (num_of_bits));
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
    if (!bit_array)
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
    if (!bit_array)
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
    if (!bit_array)
    {
        ERROR_INFO ("null struct");
        return 0;
    }

    return bit_array->num_of_bits;
}

//--------

void bit_array_dump (my_bit_array* bit_array)
{
    if (!bit_array)
    {
        ERROR_INFO ("null struct");
        return;
    }    

    FILE* dump = MY_FOPEN(DUMP_FILE_BIT_ARRAY, "wb");
    if (!dump)
    {
        ERROR_INFO ("Incorrect open of dump");
        return;
    }

    fprintf (dump,     "|-------------------------------------|\n"
                       "|           DUMP BIT ARRAY            |\n"
                       "|-------------------------------------|\n"
                       "\n"
                       "    num_of_bits  = [%" PRIu64 "];\n"
                       "    capacity     = [%" PRIu64 "];\n"
                       "    buff_bits    = [%p];\n"
                       "\n"
                       "    Buffer bits:\n",
                            bit_array->num_of_bits,
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
}

//--------

void bit_array_foreach (my_bit_array* bit_array, int (*func) (uint8_t, void*), void* data)
{
    if (!bit_array)
    {
        ERROR_INFO ("null struct");
        return;
    }

    if (!func)
    {
        ERROR_INFO ("null func");
        return;        
    }

    for (bit_index_t i = 0; i < bit_array->num_of_bits; i++)
        func (bit_array_get_bit (bit_array, i), data);
}

//-----------------
// bit operations (1 bit)
//-----------------

void bit_array_set_bit (my_bit_array* bit_array, bit_index_t index)
{
    if (!bit_array)
    {
        ERROR_INFO ("null struct");
        return;
    }

    if (index > bit_array->num_of_bits)
    {
        ERROR_INFO ("index > num_of_bits");
        return;
    }

    bit_array->buff_bits[index / WORD_SIZE] |= (1 << (index % WORD_SIZE));
}

//--------

void bit_array_clear_bit (my_bit_array* bit_array, bit_index_t index)
{
    if (!bit_array)
    {
        ERROR_INFO ("null struct");
        return;
    }

    if (index > bit_array->num_of_bits)
    {
        ERROR_INFO ("index > num_of_bits");
        return;
    }

    bit_array->buff_bits[index / WORD_SIZE] &= ~(1 << (index % WORD_SIZE));
}

//--------

int bit_array_get_bit (my_bit_array* bit_array, bit_index_t index)
{
    if (!bit_array)
    {
        ERROR_INFO ("null struct");
        return 0;
    }

    if (index > bit_array->num_of_bits)
    {
        ERROR_INFO ("index > num_of_bits");
        return 0;
    }

    return ((bit_array->buff_bits[index / WORD_SIZE] & (1 << (index % WORD_SIZE))) != 0);  
}

//--------

void bit_array_toggle_bit (my_bit_array* bit_array, bit_index_t index)
{
    if (!bit_array)
    {
        ERROR_INFO ("null struct");
        return;
    }

    if (index > bit_array->num_of_bits)
    {
        ERROR_INFO ("index > num_of_bits");
        return;
    }

    bit_array->buff_bits[index / WORD_SIZE] ^= (1 << (index % WORD_SIZE));
}

//--------------
//   bit operation (region)
//--------------

void bit_array_set_region (my_bit_array* bit_array, bit_index_t start, bit_index_t length)
{
    if (!bit_array)
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

        for(word_t i_word = first_word + 1; i_word < last_word; i_word++)
            bit_array->buff_bits[i_word] = WORD_MAX;

        bit_array->buff_bits[last_word]  |=  bit_mask_64 (last_offset + 1);
    }
}

//-------

void bit_array_clear_region (my_bit_array* bit_array, bit_index_t start, bit_index_t length)
{
    if (!bit_array)
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

        for(word_t i_word = first_word + 1; i_word < last_word; i_word++)
            bit_array->buff_bits[i_word] = WORD_MIN;

        bit_array->buff_bits[last_word]  &= ~bit_mask_64 (last_offset + 1);
    }
}

//-------

void bit_array_toggle_region (my_bit_array* bit_array, bit_index_t start, bit_index_t length)
{
    if (!bit_array)
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
        bit_array->buff_bits[first_word] ^= mask;
    }
    else
    {
        // first word
        bit_array->buff_bits[first_word] ^= ~bit_mask_64 (first_offset);

        for(word_t i_word = first_word + 1; i_word < last_word; i_word++)
            bit_array->buff_bits[i_word] ^= WORD_MAX;

        bit_array->buff_bits[last_word]  ^=  bit_mask_64 (last_offset + 1);
    }
}

//--------------
//   find funcs
//--------------
/*
bit_index_t bit_array_find_first_set_bit (my_bit_array* bit_array)
{
    return bit_array_find_next_set_bit (bit_array, 0);
}

//-----

bit_index_t bit_array_find_next_set_bit (my_bit_array* bit_array, bit_index_t start)
{
    if (start > bit_array->num_of_bits)
    {
        ERROR_INFO ("index > num_of_bits");
        return 0;
    }
}

char test(const BIT_ARRAY* bitarr, bit_index_t start, bit_index_t* result)
{
    if (bitarr->num_of_bits == 0 || start >= bitarr->num_of_bits) 
        return 0;

    //Find first word that is greater than zero
    word_addr_t i = bitset64_wrd(start);
    word_t w = (bitarr->words[i]) & ~bitmask64(bitset64_idx(start));

    while (1) 
    {
        if (w > 0)
        {
            bit_index_t pos = i * WORD_SIZE + trailing_zeros(w);
            if (pos < bitarr->num_of_bits) 
            { 
                *result = pos; 
                return 1;
            }
            else 
                return 0;
        }
        
        i++;
        
        if(i >= bitarr->num_of_words)
            break;

        w = bitarr->words[i];
    }

    return 0; \
}
*/

//-----------------------------------------