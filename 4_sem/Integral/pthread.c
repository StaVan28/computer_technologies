
#define  _GNU_SOURCE    // for pthread_attr_setaffinity_np() 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>      // errno
#include <math.h>       // atan()
#include <unistd.h>     // sysconf()
#include <mm_malloc.h>  // _mm_malloc() 

#include "debug.h"
#include "pthread.h"

//------------------------------------
//      CONSTS
//------------------------------------

// DON'T TOUCH
static const pthread_t POISON_ID  = 0;
static const int       DIGIT_BASE = 10;

//------------------------------------

static ssize_t get_input_threads (const char* argv[]);

static thread_info* roundup_cache_line_alloc (size_t nmemb);
static void         roundup_cache_line_free  (thread_info* ptr);

//------------------------------------

void integral_info_construct (integral_info* int_info, const char* argv[],    \
                              long double x1, long double x2, long double step)
{
    int_info->input_threads = get_input_threads (argv);
    if (int_info->input_threads < 0)
    {
        errno = EINVAL;
        ERROR_INFO ("Launch invalid arg");
    }

    PRINT_STEP (int_info-> input_threads, %ld);

    int_info->x1 = x1;
    PRINT_STEP (int_info->x1, %lf);

    int_info->x2 = x2;
    PRINT_STEP (int_info->x2, %lf);

    int_info->step = step;
    PRINT_STEP (int_info->step, %Lf);

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

    long double delta_x = ( int_info->max_threads > int_info->online_threads ) ?
                          ( (x2 - x1) / int_info->online_threads )             :
                          ( (x2 - x1) / int_info-> input_threads )             ;

    // fill emptys treads
    ssize_t i_thread = 0;
    for (; i_thread < int_info->empty_threads; i_thread++)
    {
        int_info->buf_info_thread[i_thread] = (thread_info) {
            .id_thread  = POISON_ID,
            .num_thread = i_thread,
            .x1         = x1,
            .x2         = x1 + delta_x,
            .step       = step
        };
    }

    // fill useful treads
    for (size_t i_onl_thread = 0; i_thread < int_info->online_threads; i_thread++, i_onl_thread++)
    {
        int_info->buf_info_thread[i_thread] = (thread_info) {
            .id_thread  = POISON_ID,
            .num_thread = i_thread,
            .x1         = x1 +  i_onl_thread      * delta_x,
            .x2         = x1 + (i_onl_thread + 1) * delta_x,
            .step       = step
        };

        PRINT_STEP (int_info->buf_info_thread[i_thread].num_thread, %ld);
    }

    // empty treads > online_treads
    for (; i_thread < int_info->max_threads; i_thread++)
    {
        int_info->buf_info_thread[i_thread] = (thread_info) {
            .id_thread  = POISON_ID,
            .num_thread = i_thread % int_info->online_threads,
            .x1         = x1,
            .x2         = x1 + step,
            .step       = step
        };
    }
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
    size_t round_up_size = ( sizeof (thread_info) == size_cache_line )                       ?
                           ( (size_t) size_cache_line )                                      :
                           ( (sizeof (thread_info) / size_cache_line + 1) * size_cache_line );

    PRINT_STEP (sizeof (thread_info), %lu);
    PRINT_STEP (round_up_size       , %lu);

    return (thread_info*) _mm_malloc (nmemb * round_up_size, size_cache_line);
}

//------------------------------------

static void roundup_cache_line_free (thread_info* ptr)
{
    _mm_free (ptr);
}

//------------------------------------

static ssize_t get_input_threads (const char* argv[])
{
    char* end_digit_str = NULL;

    ssize_t inp_digit = strtol (argv[1], &end_digit_str, DIGIT_BASE);

    if (errno != 0 || *end_digit_str != '\0' || inp_digit < 1)
    {
        fprintf (stderr, "Launch: %s __numb>0__\n", argv[0]);
        return -1;
    }

    return inp_digit;
}

//------------------------------------
