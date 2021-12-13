#ifndef SHARMEM_CONFIG_H_INCLUDED
#define SHARMEM_CONFIG_H_INCLUDED

//------------------------------------------------------------

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "debug.h"

//------------------------------------------------------------

//! Semaphores

static const int NUM_SEM = 7;

enum SEM
{
    FULL,
    EMPTY,
    SYNC_RD,
    SYNC_WR,
    ALONE_RD,
    ALONE_WR,
    NUM_PROC
};

//!

static const int    DFLT_FTOK_SEED = 28;
static const char*  DFLT_FTOK_PATH = "/bin/bash";
static const mode_t DFLT_MODE      = 0660;

#define PAGE_SIZE 4096

//!

int   create_sem (void);

void  delete_sem (int id_sem);

int   create_shm (void);

void  delete_shm (int id_shm);

char*   link_shm (int id_shm);

void  unlink_shm (char* shmaddr);


char* create_tmp_buf (void);

void my_semop (int semid, struct sembuf *sops, unsigned nsops);

//------------------------------------------------------------

#endif // SHARMEM_CONFIG_H_INCLUDED