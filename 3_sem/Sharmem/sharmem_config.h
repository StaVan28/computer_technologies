#ifndef FIFO_CONFIG_H_INCLUDED
#define FIFO_CONFIG_H_INCLUDED

//------------------------------------------------------------

#define _GNU_SOURCE
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
#include <sys/shm.h>
#include <sys/sem.h>

//------------------------------------------------------------

static const mode_t DFLT_FIFO_MODE = 0666;

#define BUFF_SIZE 4096

//!

int  synchr_fifo       (const char* fifo_path, int flags);

void data_writing_fifo (int from_fd, int to_fd);

//------------------------------------------------------------

#endif // FIFO_CONFIG_H_INCLUDED