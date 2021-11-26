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

int  create_msg   (void);

void delete_msg   (int id);

void send_msg     (int id, long type);

void recv_msg     (int id, long type, struct my_msg* get_msg);

void create_procs (int* i_proc, int num_proc);

//--------------------------------------------

#endif // QUEUE_H_INCLUDED