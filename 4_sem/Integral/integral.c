#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sched.h>              // CPU_*
#include <stdint.h>
#include <math.h>

#include "debug.h"
#include "integral.h"
#include "pthread.h"

//-----------------------------------------

// USER FUNC
static long double integral_func (long double x)   
{
    return x * atan (x);
}

//--------

void* integral (void* ptr)
{
    thread_info* thr_info = (thread_info*)(uintptr_t) ptr;

    pthread_t thread = pthread_self ();

    cpu_set_t cpu;
    CPU_ZERO (&cpu);

    PRINT_STEP (thr_info->num_thread, %ld);
    CPU_SET (thr_info->num_thread, &cpu);

    int ret_setaffinity = pthread_setaffinity_np (thread, sizeof (cpu_set_t), &cpu);
    if (ret_setaffinity < 0)
        ERROR_INFO ("pthread_setaffinity_np");

    for (long double x = thr_info->x1; x < thr_info->x2; x += thr_info->step)
        thr_info->sum += integral_func (x) * thr_info->step;

    return NULL;
}

//-----------------------------------------
