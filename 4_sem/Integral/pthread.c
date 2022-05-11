
#define  _GNU_SOURCE    // for pthread_attr_setaffinity_np() 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>      // errno
#include <math.h>       // atan()
#include <unistd.h>     // sysconf()

#include "integral.h"
#include "debug.h"
#include "pthread.h"

//------------------------------------
//      CONSTS
//------------------------------------

// DON'T TOUCH
static const pthread_t POISON_ID    =  0;

//------------------------------------

static ssize_t get_input_threads (const char* argv[]);

static thread_info* roundup_cache_line_alloc (size_t nmemb);
static void         roundup_cache_line_free  (thread_info* ptr);

//------------------------------------

void integral_info_construct (integral_info* int_info, const char* argv[])
{
    int_info->input_threads = get_input_threads (argv);
    if (int_info-> input_threads < 0)
        ERROR_INFO ("int_info->input_threads");

    PRINT_STEP (int_info-> input_threads, %ld);

    int_info->online_threads = sysconf (_SC_NPROCESSORS_ONLN);
    if (int_info->online_threads < 0)
        ERROR_INFO ("int_info->online_threads");

    PRINT_STEP (int_info->online_threads, %ld);

    if (int_info->input_threads >= int_info->online_threads)
    {
        int_info->empty_threads = 0;
        int_info->  max_threads = int_info->input_threads;
    }
    else
    {
        int_info->empty_threads = int_info->online_threads - int_info->input_threads;
        int_info->  max_threads = int_info->online_threads;
    }

    PRINT_STEP (int_info->empty_threads, %ld);
    PRINT_STEP (int_info->  max_threads, %ld);

    int_info->buf_info_thread = roundup_cache_line_alloc (int_info->max_threads);
    if (int_info->buf_info_thread == NULL)
        ERROR_INFO ("roundup_cache_line_alloc");

    // fill emptys treads
    ssize_t i_thread = 0;
    for (; i_thread < int_info->empty_threads; i_thread++)
    {
        int_info->buf_info_thread[i_thread] = (thread_info) {
            .id_thread  = POISON_ID,
            .num_thread = i_thread,
            .x1         = X1,
            .x2         = X1 + STEP_X,
            .step       = STEP_X
        };
    }

    long double delta_x = (X2 - X1) / int_info->input_threads;

    // fill useful treads
    for (size_t i_onl_thread = 0; i_thread < int_info->max_threads; i_thread++, i_onl_thread++)
    {
        int_info->buf_info_thread[i_thread] = (thread_info) {
            .id_thread  = POISON_ID,
            .num_thread = i_thread % int_info->online_threads,
            .x1         = X1 +  i_onl_thread      * delta_x,
            .x2         = X1 + (i_onl_thread + 1) * delta_x,
            .step       = STEP_X
        };

        PRINT_STEP (int_info->buf_info_thread[i_thread].num_thread, %ld);
    }

    int_info->x1 = X1;
    PRINT_STEP (int_info->x1, %lf);

    int_info->x2 = X2;
    PRINT_STEP (int_info->x2, %lf);

    int_info->step = STEP_X;
    PRINT_STEP (int_info->step, %Lf);
}

//------------------------------------

void integral_info_destruct (integral_info* int_info)
{
    roundup_cache_line_free (int_info->buf_info_thread);
}

//------------------------------------

static thread_info* roundup_cache_line_alloc (size_t nmemb)
{
    ssize_t size_cache_line = sysconf (_SC_LEVEL1_DCACHE_LINESIZE);
    if (size_cache_line < 0)
        ERROR_INFO ("size_cache_line");

    // round up
    size_t round_up_size = (sizeof (thread_info) / size_cache_line + 1) * size_cache_line;

    return (thread_info*) calloc (nmemb, round_up_size);
}

//------------------------------------

static void roundup_cache_line_free (thread_info* ptr)
{
    free (ptr);
}

//------------------------------------

static ssize_t get_input_threads (const char* argv[])
{
    char* end_digit_str = NULL;

    ssize_t inp_digit = strtol (argv[1], &end_digit_str, DIGIT_BASE);

    if (errno != 0)
    {
        perror ("ERROR! strtol(): smth went wrong...");
        return -1;
    }

    if (*end_digit_str != '\0')
    {
        perror ("ERROR! strtol(): not a number...");
        return -1;
    }

    if (inp_digit < 1)
    {
        perror ("ERROR! Please, input natural number\n");
        return -1;
    }

    return inp_digit;
}

//------------------------------------
