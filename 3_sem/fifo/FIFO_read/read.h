#ifndef FIFO_READ_H_INCLUDED
#define FIFO_READ_H_INCLUDED

//------------------------------------------------------------

#include "fifo_config.h"

//------------------------------------------------------------

void        read_fifo        (void);

const char* generate_name    (void);

int         create_secr_fifo (const char* secr_name, int flags);

void        write_to_fifo    (int wr_fd_fifo, const char* secr_name);

bool        is_can_read_fifo (int secr_fd_fifo);

//------------------------------------------------------------

#endif // FIFO_READ_H_INCLUDED