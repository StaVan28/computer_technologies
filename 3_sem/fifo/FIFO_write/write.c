#include "write.h"

//---------------------------------------------------------------------

void rm_fifo(void);

//---------------------------------------------------------------------

void write_fifo(const char* file_path)
{
	int wr_fifo    = 0;
	int wr_fd_fifo = 0;
	int rd_fd_file = 0;
	
	if ((rd_fd_file = open(file_path, O_RDONLY)) < 0)
	{
		fprintf(stderr, "ERROR! Smth error with open()\n");
		exit(EXIT_FAILURE);		
	}

	if ((wr_fifo = mkfifo(DFLT_FIFO_PATH, DFLT_FIFO_MODE | O_EXCL)) != 0)
	{
		fprintf(stderr, "ERROR! Smth error with mkfifo()\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		atexit(rm_fifo);
	}

	if ((wr_fd_fifo = open(DFLT_FIFO_PATH, O_WRONLY | O_NONBLOCK)) < 0)
	{
		fprintf(stderr, "ERROR! Smth error with open()\n");
		exit(EXIT_FAILURE);		
	}

	if (fcntl(wr_fd_fifo, F_SETFL, O_WRONLY) < 0)
	{
		fprintf(stderr, "ERROR! Smth error with fcntl()\n");
		exit(EXIT_FAILURE);	
	}

	int buff[BUFF_SIZE] = {};
	int num_symb        = 0;

	while ((num_symb = read(rd_fd_file, buff, BUFF_SIZE)) > 0)
	{
		if (write(wr_fd_fifo, buff, num_symb) != num_symb)
		{
			fprintf(stderr, "ERROR! Something wrong with write()\n");
			exit(EXIT_FAILURE);
		}
	}

	if (num_symb < 0)
	{
		fprintf(stderr, "ERROR! Something wrong with read()\n");
		exit(EXIT_FAILURE);		
	}

	close(wr_fd_fifo);
	close(rd_fd_file);
}

//---------------------------------------------------------------------

void rm_fifo(void)
{
	unlink(DFLT_FIFO_PATH);
}