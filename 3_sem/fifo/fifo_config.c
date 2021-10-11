#include "fifo_config.h"

//------------------------------------------------------------

int synchr_fifo (const char* fifo_path, int flags)
{
    int fd_fifo = 0;

    if ((mkfifo (fifo_path, DFLT_FIFO_MODE) < 0) && (errno != EEXIST))
    {
        perror ("ERROR! Smth with mkfifo()\n");
        exit   (EXIT_FAILURE);
    }

    errno = 0;

    if ((fd_fifo = open (fifo_path, flags)) < 0)
    {
        perror ("ERROR! Smth with open()\n");
        exit   (EXIT_FAILURE);
    }

    return fd_fifo;
}

//---------------------------------------------------------------------

const char* create_name (pid_t secr_pid)
{
    char* name_buff = (char*) calloc (MAX_NAME, 1);
    if   (name_buff == NULL)
    {
        perror ("ERROR! Smth error with calloc()\n");
        exit   (EXIT_FAILURE);        
    }

    sprintf (name_buff, "/tmp/secr_name_%d", secr_pid);

    return name_buff;
}

//------------------------------------------------------------

void data_writing_fifo (int from_fd, int to_fd)
{
    int buff[BUFF_SIZE] = {};
    int num_symb        = 0;

    while ((num_symb = read (from_fd, buff, BUFF_SIZE)) > 0)
    {
        if (write (to_fd, buff, num_symb) != num_symb)
        {
            perror ("ERROR! Something wrong with write()\n");
            exit   (EXIT_FAILURE);
        }
    }

    if (num_symb < 0)
    {
        perror ("ERROR! Something wrong with read()\n");
        exit   (EXIT_FAILURE);        
    }
}

//------------------------------------------------------------