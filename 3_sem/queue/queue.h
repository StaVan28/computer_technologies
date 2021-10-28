#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

//--------------------------------------------

#include "config.h"

//!

static const char* DFLT_PATH     = ".";
static const int   DFLT_SEED     = 28;
static const int   DFLT_MSG_MODE = 0660;

//!

struct my_msg
{
   long type;
};

//!

int  create_msg (void);

void delete_msg (int id);

//--------------------------------------------

#endif // QUEUE_H_INCLUDED