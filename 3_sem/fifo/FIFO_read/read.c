#include "read.h"

//---------------------------------------------------------------------

void rm_fifo(void);

//---------------------------------------------------------------------

void read_fifo(void)
{
    int rd_fd_fifo = synchr_fifo(O_RDONLY);

    data_writing_fifo(rd_fd_fifo, STDOUT_FILENO);

    close(rd_fd_fifo);
    unlink(DFLT_FIFO_PATH);
}

//---------------------------------------------------------------------