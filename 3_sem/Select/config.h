#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

//--------------------------------------------

#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>
#include <limits.h>
#include <stdbool.h>
#include <sys/prctl.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <signal.h> 

//--------------------------------------------

int get_num (const int argc, const char *argv[]);

//--------------------------------------------

#endif // CONFIG_H_INCLUDED