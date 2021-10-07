#include "fifo_config.h"

//------------------------------------------------------------

int synchr_fifo (const char* fifo_path, int flags)
{
    int fifo    = 0;
    int fd_fifo = 0;

    if (access (fifo_path, F_OK) == -1)
    {
        printf("(____)open()   {synchr_fifo_if}\n\n");

        if ((fd_fifo = open (fifo_path, flags)) < 0)
        {
            fprintf (stderr, "ERROR! Smth error with open()\n");
            exit (EXIT_FAILURE);
        }   
    }   
    else
    {
        printf("(____)mkfifo() {synchr_fifo_else}\n");
        printf("(____)open()   {synchr_fifo_else}\n\n");

        if ((fifo = mkfifo (fifo_path, DFLT_FIFO_MODE)) != 0)
        {
            fprintf (stderr, "ERROR! Smth error with mkfifo()\n");
            exit (EXIT_FAILURE);
        }

        if ((fd_fifo = open (fifo_path, flags)) < 0)
        {
            fprintf (stderr, "ERROR! Smth error with open()\n");
            exit (EXIT_FAILURE);
        }
    }

    return fd_fifo;
}

//---------------------------------------------------------------------

const char* create_name (pid_t secr_pid)
{
    char* name_buff = (char*) calloc (1, MAX_NAME);
    if   (name_buff == NULL)
    {
        fprintf (stderr, "ERROR! Smth error with calloc()\n");
        exit (EXIT_FAILURE);        
    }

    sprintf (name_buff, "./secr_name_%d", secr_pid);

    return name_buff;
}

//------------------------------------------------------------

void data_writing_fifo (int from_fd, int to_fd)
{
    int buff[BUFF_SIZE] = {};
    int num_symb        = 0;

    while ((num_symb = read (from_fd, buff, BUFF_SIZE)) > 0)
    {
        printf("num_symb(while) = %d\n", num_symb);


        int write_num = 0;
        if ((write_num = write (to_fd, buff, num_symb)) != num_symb)
        {
            fprintf (stderr, "ERROR! Something wrong with write()\n");
            exit (EXIT_FAILURE);
        }
        
        printf("write_num = %d\n", write_num);
    }

    printf("num_symb = %d\n", num_symb);

    if (num_symb < 0)
    {
        fprintf (stderr, "ERROR! Something wrong with read()\n");
        exit (EXIT_FAILURE);        
    }
}

//------------------------------------------------------------