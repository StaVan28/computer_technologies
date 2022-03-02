
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include "bit_array.h"

//-----------------------------------------

#define WORD_SIZE           64
#define MIN_CAPACITY        2

//-----

const char* DUMP_FILE_BIT_ARRAY = "dump_bit_array.txt";

//-----

#define ERROR_INFO(err_str)                                      \
                perror  (        "ERROR! " err_str);             \
                fprintf (stderr, "errno = %d\n", errno);         \
                                                                 \
                fprintf (stderr, "file = {%s}, line = {%d}\n",   \
                                  __FILE__,    __LINE__       ); \

//-----

#define ROUNDUP_BITS_TO_WORDS(bits) (((bits) + 63) / 64)

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
    bit_array->num_of_bits  = num_of_bits;

    word_t word64_to_pow2 = roundup_word64_to_pow2 (ROUNDUP_BITS_TO_WORDS (num_of_bits));
    if (word64_to_pow2 > MIN_CAPACITY)
        bit_array->capacity = word64_to_pow2;
    else
        bit_array->capacity = MIN_CAPACITY;

    bit_array->buff_bits = (word_t*) calloc (bit_array->capacity, sizeof (word_t));
    if (bit_array->buff_bits == NULL)
    {
        errno = ENOMEM;
        return NULL;
    }

    return bit_array;
}

//--------

void bit_array_deinit (my_bit_array* bit_array)
{
    free (bit_array->buff_bits);
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
    if (bit_array->buff_bits != NULL)
        bit_array_deinit (bit_array);

    free (bit_array);
    bit_array = NULL;
}

//--------

bit_index_t bit_array_length (my_bit_array* bit_array)
{
    return bit_array->num_of_bits;
}

//--------

void bit_array_dump (my_bit_array* bit_array)
{
    FILE* dump = fopen (DUMP_FILE_BIT_ARRAY, "wb");
    if (!dump)
    {
        ERROR_INFO ("Incorrect open of dump");
        exit       (EXIT_FAILURE);
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

void bit_array_set_bit (my_bit_array* bit_array, bit_index_t index)
{
    if (index > bit_array->num_of_bits)
    {
        ERROR_INFO ("index > bit_array->num_of_bits");
        return;
    }

    bit_array->buff_bits[index / WORD_SIZE] |= (1 << (index % WORD_SIZE));
}

//--------

void bit_array_clear_bit (my_bit_array* bit_array, bit_index_t index)
{
    if (index > bit_array->num_of_bits)
    {
        ERROR_INFO ("index > bit_array->num_of_bits");
        return;
    }

    bit_array->buff_bits[index / WORD_SIZE] &= ~(1 << (index % WORD_SIZE));
}

//--------

int bit_array_get_bit (my_bit_array* bit_array, bit_index_t index)
{
    if (index > bit_array->num_of_bits)
    {
        ERROR_INFO ("index > bit_array->num_of_bits");
        return 0;
    }

    return ((bit_array->buff_bits[index / WORD_SIZE] & (1 << (index % WORD_SIZE))) != 0);  
}

//--------

void bit_array_toggle_bit (my_bit_array* bit_array, bit_index_t index)
{
    if (index > bit_array->num_of_bits)
    {
        ERROR_INFO ("index > bit_array->num_of_bits");
        return;
    }

    bit_array->buff_bits[index / WORD_SIZE] ^= (1 << (index % WORD_SIZE));
}

//-----------------------------------------