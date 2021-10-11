#ifndef FIFO_READ_H_INCLUDED
#define FIFO_READ_H_INCLUDED

//------------------------------------------------------------

#include "fifo_config.h"

//------------------------------------------------------------

void read_fifo         (void);

int  create_secr_fifo  (const char* secr_name, int flags);

void transfer_pid_fifo (int dflt_fifo, pid_t secr_pid);

bool is_can_read_fifo  (int secr__fifo);

//------------------------------------------------------------

#endif // FIFO_READ_H_INCLUDED