#include "fifo_config.h"

//------------------------------------------------------------

static void rm_fifo(void);

//------------------------------------------------------------

int synchr_fifo(int flags)
{
    int fifo    = 0;
    int fd_fifo = 0;

    if ((fd_fifo = open(DFLT_FIFO_PATH, flags)) > 0)
    {
    	return fd_fifo;
    }   
    else
    {
        if ((fifo = mkfifo(DFLT_FIFO_PATH, DFLT_FIFO_MODE | O_EXCL)) != 0)
        {
            fprintf(stderr, "ERROR! Smth error with mkfifo()\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            if (atexit(rm_fifo) != 0)
            {
                fprintf(stderr, "ERROR! Smth error with atexit()\n");
                exit(EXIT_FAILURE);
            }
        }

        if ((fd_fifo = open(DFLT_FIFO_PATH,flags)) < 0)
        {
            fprintf(stderr, "ERROR! Smth error with open()\n");
            exit(EXIT_FAILURE);
        }
    }

    return fd_fifo;
}

//------------------------------------------------------------

void data_writing_fifo(int from_fd, int to_fd)
{
    int buff[BUFF_SIZE] = {};
    int num_symb        = 0;

    while ((num_symb = read(from_fd, buff, BUFF_SIZE)) > 0)
    {
        if (write(to_fd, buff, num_symb) != num_symb)
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
}

//------------------------------------------------------------

static void rm_fifo(void)
{
    unlink(DFLT_FIFO_PATH);
}

//------------------------------------------------------------