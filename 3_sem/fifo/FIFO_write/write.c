#include "write.h"

//---------------------------------------------------------------------

void write_fifo (const char* file_path)
{
    int rd_fd_fifo = synchr_fifo (DFLT_FIFO_PATH, O_RDONLY);

    pid_t       secr_pid  = read_from_fifo (rd_fd_fifo);
    const char* secr_name = create_name (secr_pid);
    printf ("secr_name = {%s}\n", secr_name);
    
    int  secr_fd_fifo = 0;
    if ((secr_fd_fifo = open ("./secr_name", O_WRONLY)) < 0)
    {
        fprintf (stderr, "ERROR! Smth error with open()_1\n");
        exit (EXIT_FAILURE);     
    }

    int  rd_fd_file = 0;    
    if ((rd_fd_file = open (file_path, O_RDONLY)) < 0)
    {
        fprintf (stderr, "ERROR! Smth error with open()\n");
        exit (EXIT_FAILURE);
    }

    data_writing_fifo (rd_fd_file, secr_fd_fifo);
}

//---------------------------------------------------------------------

pid_t read_from_fifo (int rd_fd_fifo)
{
    int   num_symb = 0;
    pid_t pid;

    while ((num_symb = read (rd_fd_fifo, &pid, sizeof(pid_t))) > 0);
    if     (num_symb < 0)
    {
        fprintf (stderr, "ERROR! Something wrong with read()\n");
        exit (EXIT_FAILURE);        
    }

    return pid;
}

//---------------------------------------------------------------------