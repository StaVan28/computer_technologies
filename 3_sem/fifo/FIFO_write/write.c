#include "write.h"

//---------------------------------------------------------------------

void write_fifo (const char* file_path)
{
    int rd_fd_fifo = synchr_fifo (DFLT_FIFO_PATH, O_RDONLY);

    const char* secr_name = read_from_fifo (rd_fd_fifo);
    
    int secr_fd_fifo = synchr_fifo (secr_name, O_WRONLY | O_NONBLOCK);

    int  rd_fd_file = 0;    
    if ((rd_fd_file = open (file_path, O_RDONLY)) < 0)
    {
        fprintf (stderr, "ERROR! Smth error with open()\n");
        exit (EXIT_FAILURE);
    }

    data_writing_fifo (rd_fd_file, secr_fd_fifo);
}

//---------------------------------------------------------------------

const char* read_from_fifo (int rd_fd_fifo)
{
    int   num_symb = 0;
    char* buff     = (char*) calloc (1, MAX_NAME); 

    while ((num_symb = read (rd_fd_fifo, buff, MAX_NAME)) > 0);
    if     (num_symb < 0)
    {
        fprintf (stderr, "ERROR! Something wrong with read()\n");
        exit (EXIT_FAILURE);        
    }

    return buff;
}

//---------------------------------------------------------------------