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

//---------------------------------------------------

const char* DFLT_FIFO = "./general_fifo";
const int   DFLT_MODE = 0666;

//!

#define BUFF_SIZE 4096

//---------------------------------------------------

int main (int argc, char const *argv[])
{
	int  rd_fifo = 0;
	if ((rd_fifo = open (DFLT_FIFO, O_RDONLY)) < 0)
	{
		fprintf (stderr, "ERROR! Smth with open()\n");
		exit (EXIT_FAILURE);
	}

	int  num_symbols     = 0;
	char buff[BUFF_SIZE] = {};

	while ((num_symbols = read (rd_fifo, buff, BUFF_SIZE)) > 0)
	{
		if (write (STDOUT_FILENO, buff, BUFF_SIZE) != num_symbols)
		{
			fprintf (stderr, "ERROR! Smth with write()\n");
			exit (EXIT_FAILURE);			
		}
	}

	if (num_symbols < 0)
	{
		fprintf (stderr, "ERROR! Smth with read()\n");
		exit (EXIT_FAILURE);
	}

	return 0;
}