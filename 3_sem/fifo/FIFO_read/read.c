#include "read.h"

//---------------------------------------------------------------------

static const int RAND_SIZE = 100000000;
static const int WAIT_TIME = 5;

//---------------------------------------------------------------------

void read_fifo (void)
{
    const char* secr_name = generate_name();
   
    int secr_fd_fifo = create_secr_fifo (secr_name, O_RDONLY | O_NONBLOCK);

    if (fcntl(secr_fd_fifo, F_SETFL, O_RDONLY) < 0)
    {
        fprintf (stderr, "ERROR! Smth error with fcntl()\n");
        exit (EXIT_FAILURE);    
    }

    int wr_fd_fifo = synchr_fifo (DFLT_FIFO_PATH, O_WRONLY);

    write_to_fifo (wr_fd_fifo, secr_name);
/*
    if (!is_can_read_fifo (secr_fd_fifo))
    {
        fprintf (stderr, "ERROR! Time out!\n");
        exit (EXIT_FAILURE);  
    }
*/
    data_writing_fifo (secr_fd_fifo, STDOUT_FILENO);

    unlink (secr_name);
}

//---------------------------------------------------------------------

const char* generate_name (void)
{
    pid_t cur_pid = getpid();

    srand (time (NULL));

    int secret_digit = rand() % RAND_SIZE;

    char* name_buff = (char*) calloc (1, MAX_NAME);
    if   (name_buff == NULL)
    {
        fprintf (stderr, "ERROR! Smth error with calloc()\n");
        exit (EXIT_FAILURE);        
    }

    sprintf (name_buff, "./secr_fifo_%d", secret_digit, cur_pid);

    return name_buff;
}

//---------------------------------------------------------------------

int create_secr_fifo (const char* secr_name, int flags)
{
    int    fifo = 0;
    int fd_fifo = 0;

    if ((fifo = mkfifo (secr_name, DFLT_FIFO_MODE)) != 0)
    {
        fprintf (stderr, "ERROR! Smth error with mkfifo()\n");
        exit (EXIT_FAILURE);
    }

    if ((fd_fifo = open (secr_name, flags)) < 0)
    {
        fprintf (stderr, "ERROR! Smth error with open()\n");
        exit (EXIT_FAILURE);     
    }

    return fd_fifo;
}

//---------------------------------------------------------------------

void write_to_fifo (int wr_fd_fifo, const char* secr_name)
{
    if (write (wr_fd_fifo, secr_name, MAX_NAME) < 0)
    {
        fprintf (stderr, "ERROR! Something wrong with write()\n");
        exit (EXIT_FAILURE);        
    }
}

//---------------------------------------------------------------------

bool is_can_read_fifo (int secr_fd_fifo)
{
    int num_fd = secr_fd_fifo + 1;

    fd_set read_fds = {};
    FD_ZERO (              &read_fds);
    FD_SET  (secr_fd_fifo, &read_fds);

    struct timeval tv_fifo = {};
    tv_fifo.tv_sec  = WAIT_TIME;
    tv_fifo.tv_usec = 0;

    int ret_select = select (num_fd, &read_fds, NULL, NULL, &tv_fifo);
    if (ret_select <= 0)
        return false;
    else
        return true;
}

//---------------------------------------------------------------------