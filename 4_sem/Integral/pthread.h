
#ifndef PTHREAD_H_INCLUDED
#define PTHREAD_H_INCLUDED

//------------------------------------------------------------

#include <pthread.h>
#include <stdlib.h>

//------------------------------------------------------------

// struct for every_thread
typedef struct
{
    pthread_t  id_thread;       // returned by pthread_create ()
    ssize_t   num_thread;       // logical number of thread

    // data for current task
    long double x1;             // local border of thread
    long double x2;             // local border of thread

    long double step;
    long double sum;            // local sum

} thread_info;

//-----

// main struct of proj
typedef struct
{
    // topology
    ssize_t  input_threads;             // input number
    ssize_t online_threads;             // num of online threads
    ssize_t  empty_threads;             // threads for making linear dependence
    ssize_t    max_threads;             // max threads for working

    thread_info* buf_info_thread;       // !!Need to round up on size of cache line 

    long double x1;
    long double x2;

    long double step;
    long double sum;

} integral_info;

//------------------------------------

void integral_info_construct (integral_info* int_info, const char* argv[],    \
                              long double x1, long double x2, long double step);
void integral_info_destruct  (integral_info* int_info);

//------------------------------------------------------------

#endif // PTHREAD_H_INCLUDED