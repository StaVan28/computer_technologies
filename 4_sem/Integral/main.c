
#include <stdio.h>

#include "integral.h"
#include "debug.h"
#include "pthread.h"

//------------------------------------

// DON'T TOUCH
static const char STDIN_ARGC = 2;

//------------------------------------

int main (const int argc, const char* argv[])
{
    if (argc != STDIN_ARGC)
    {
        fprintf (stderr, "ERROR! Execute: %s __number__\n", argv[0]);
        exit    (EXIT_FAILURE);
    }

    integral_info int_info = {};
    integral_info_construct (&int_info, argv, X1, X2, STEP_X);

    for (ssize_t i_thread = 0; i_thread < int_info.max_threads; i_thread++)
    {
        int ret_pthread_create = pthread_create ( &(int_info.buf_info_thread[i_thread].id_thread),  \
                                                  NULL,                                             \
                                                  integral,                                         \
                                                  &(int_info.buf_info_thread[i_thread]) );
        if (ret_pthread_create != 0)
            ERROR_INFO ("pthread_create");
    }

    for (ssize_t i_thread = 0; i_thread < int_info.max_threads; i_thread++)
    {
        int ret_pthread_join = pthread_join (int_info.buf_info_thread[i_thread].id_thread, NULL);
        if (ret_pthread_join < 0)
            ERROR_INFO ("pthread_join");

        if (i_thread >= int_info.empty_threads)
            int_info.sum += int_info.buf_info_thread[i_thread].sum;
    }

    printf ("\nSUM [%Lf]\n", int_info.sum);

    integral_info_destruct (&int_info);
}

//------------------------------------

//
// Optimizations:
// 1) divide all threds to 
//     a) empty, but work  
//     b) useful
//     c) empty, not work
// 2) create local params in integral func
// 3) round up cache on CACHE LINE SIZE 
// 4) main idea:
//     all working threads must do the same amount of work.
//     (include empty, but do not take into account their amount)
// 