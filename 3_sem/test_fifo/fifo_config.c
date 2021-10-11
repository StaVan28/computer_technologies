#include "fifo_config.h"

//---------------------------------------------------

int synch_fifo (const char* fifo_path, int flags)
{
    int fifo_fd = 0;

    if (mkfifo (fifo_path, DFLT_MODE) != 0 && errno != EEXIST)
    {
        fprintf (stderr, "ERROR! Smth with mkfifo()\n");
        exit (EXIT_FAILURE);
    }

    if ((fifo_fd = open (fifo_path, flags)) < 0)
    {
        fprintf (stderr, "ERROR! Smth with open()\n");
        exit (EXIT_FAILURE);
    }

    errno = 0;

    return fifo_fd;
}

//---------------------------------------------------

void write_data (int from_fd, int to_fd)
{
    int  num_symbols     = 0;
    char buff[BUFF_SIZE] = {};

    while ((num_symbols = read (from_fd, buff, BUFF_SIZE)) > 0)
    {
        if (write (to_fd, buff, num_symbols) != num_symbols)
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
}

//---------------------------------------------------

const char* create_name (pid_t cur_pid)
{
    char* name_buff = (char*) calloc (1, 25);
    if   (name_buff == NULL)
    {
        fprintf (stderr, "ERROR! Smth error with calloc()\n");
        exit (EXIT_FAILURE);        
    }

    sprintf (name_buff, "./secr_name_%d", cur_pid);

    return name_buff;
}
