#include "write.h"

//---------------------------------------------------------------------

void write_fifo(const char* file_path)
{
    int wr_fd_fifo = synchr_fifo(O_WRONLY);
    
    

    int rd_fd_file = 0;
    if ((rd_fd_file = open(file_path, O_RDONLY)) < 0)
    {
        fprintf(stderr, "ERROR! Smth error with open()\n");
        exit(EXIT_FAILURE);
    }

    data_writing_fifo(rd_fd_file, wr_fd_fifo);

    close(wr_fd_fifo);
    close(rd_fd_file);
}

//---------------------------------------------------------------------
