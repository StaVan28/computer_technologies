#ifndef FIFO_WRITE_H_INCLUDED
#define FIFO_WRITE_H_INCLUDED

//------------------------------------------------------------

#include "fifo_config.h"

//------------------------------------------------------------

void  write_fifo   (const char* file_path);

pid_t get_pid_fifo (int rd_fd_fifo);

//------------------------------------------------------------

#endif // FIFO_WRITE_H_INCLUDED