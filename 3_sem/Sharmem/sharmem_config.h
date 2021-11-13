#ifndef SHARMEM_CONFIG_H_INCLUDED
#define SHARMEM_CONFIG_H_INCLUDED

//------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

#include "debug.h"

//------------------------------------------------------------

//! Semaphores

static const int NUM_SEM = 2;

//

static const int    DFLT_FTOK_SEED = 28;
static const char*  DFLT_FTOK_PATH = "../test.txt";
static const mode_t DFLT_MODE      = 0660;

#define PAGE_SIZE 4096

//!

int   create_sem (void);

void  delete_sem (int id_sem);

int   create_shm (void);

void  delete_shm (int id_shm);

char*   link_shm (int id_shm);

void  unlink_shm (char* shmaddr);

//------------------------------------------------------------

#endif // SHARMEM_CONFIG_H_INCLUDED