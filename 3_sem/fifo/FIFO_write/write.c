#include "write.h"

//---------------------------------------------------------------------

static const int MAX_NAME = 20;

//---------------------------------------------------------------------

void write_fifo(const char* file_path)
{
    int         wr_fd_fifo = synchr_fifo(O_WRONLY);
    const char* secr_name  = generate_name();

    int  rd_fd_file = 0;
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

const char* generate_name(void)
{
    pid_t cur_pid = getpid();

    srand(time(NULL));

    int secret_digit = rand() % 100000000;

    char* name_buff = (char*) calloc(1, MAX_NAME);
    if   (name_buff == NULL)
    {
        fprintf(stderr, "ERROR! Smth error with calloc()\n");
        exit(EXIT_FAILURE);        
    }

    sprintf(name_buff, "%d%d", secret_digit, cur_pid);

    return name_buff;
}


//---------------------------------------------------------------------