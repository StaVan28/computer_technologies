#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

//--------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>

//--------------------------------------------

static const int STDIN_ARGC = 2;
static const int DIGIT_BASE = 10;

//!

int parsing_input (const int argc, const char *argv[]);

//--------------------------------------------

#endif // CONFIG_H_INCLUDED