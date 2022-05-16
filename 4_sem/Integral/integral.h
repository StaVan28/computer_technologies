
#ifndef INTEGRAL_H_INCLUDED
#define INTEGRAL_H_INCLUDED

//-----------------------------------------

#include <stdint.h>

//------------------------------------
//      CONSTS
//------------------------------------

// DON'T TOUCH
static const ssize_t POISON_NUM_THREAD = -1;

//-----------------------------------------

// USER SETTING
static const long double     X1 = 1;
static const long double     X2 = 18;
static const long double STEP_X = 0.0000001;

//-----------------------------------------

// for pthread_create ()
void* integral (void* ptr);

//-----------------------------------------

#endif // INTEGRAL_H_INCLUDED
