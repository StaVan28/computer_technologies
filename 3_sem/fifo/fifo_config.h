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

//------------------------------------------------------------

static const mode_t DFLT_FIFO_MODE = 0666;
static const char*  DFLT_FIFO_PATH = "./my_fifo";

#define BUFF_SIZE 4096
#define MAX_NAME  16

//!

int  synchr_fifo       (const char* fifo_path, int flags);

void data_writing_fifo (int from_fd, int to_fd);

//------------------------------------------------------------

#endif // FIFO_CONFIG_H_INCLUDED