#ifndef FIFO_CONFIG_H_INCLUDED
#define FIFO_CONFIG_H_INCLUDED

//------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>

//------------------------------------------------------------

static const char* DFLT_FIFO = "./general_fifo";
static const char* SECR_FIFO = "./secr_fifo"; 
static const int   DFLT_MODE = 0666;

//!

#define BUFF_SIZE   4096

//!

void        write_data  (int from_fd, int to_fd);

int         synch_fifo  (const char* fifo_path, int flags);

const char* create_name (pid_t cur_pid);

//------------------------------------------------------------

#endif // FIFO_CONFIG_H_INCLUDED