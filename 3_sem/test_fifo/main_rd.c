#include "fifo_config.h"
//---------------------------------------------------

static const int WAIT_TIME = 5;

//!

bool is_can_read_fifo (int secr_fd_fifo);

void transfer_pid_fifo (int dflt_fifo, pid_t secr_pid);

//---------------------------------------------------

int main (int argc, char const *argv[])
{
    //! CREATE SECR NAME

    pid_t       secr_pid  = getpid();
    const char* secr_name = create_name (secr_pid);
    
    //! CREATE AND CONNECT WITH SECR FIFO

    if (mkfifo (secr_name, DFLT_MODE) != 0 && errno != EEXIST)
    {
        fprintf (stderr, "ERROR! Smth with mkfifo()\n");
        exit (EXIT_FAILURE);
    }

    errno = 0;

    int  secr_fifo = 0;
    if ((secr_fifo = open (secr_name, O_RDONLY | O_NONBLOCK)) < 0)
    {
        fprintf (stderr, "ERROR! Smth with open()\n");
        exit (EXIT_FAILURE);
    }
    
    //! CONNECT WITH DFLT FIFO

    if (mkfifo (DFLT_FIFO, DFLT_MODE) != 0 && errno != EEXIST)
    {
        fprintf (stderr, "ERROR! Smth with mkfifo()\n");
        exit (EXIT_FAILURE);
    }

    errno = 0;

    int  dflt_fifo = 0;
    if ((dflt_fifo = open (DFLT_FIFO, O_WRONLY)) < 0)
    {
        fprintf (stderr, "ERROR! Smth with open()\n");
        exit (EXIT_FAILURE);
    }

    transfer_pid_fifo (dflt_fifo, secr_pid);

    // WAIT main_wr.c

    if(!is_can_read_fifo (secr_fifo))
    {
        fprintf (stderr, "ERROR! Time out...\n");
        exit (EXIT_FAILURE);
    }
    
    //! WRITE TO STDOUT

    write_data (secr_fifo, STDOUT_FILENO);

    close(secr_fifo);
    close(dflt_fifo);

    return 0;
}

//---------------------------------------------------

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

//---------------------------------------------------

void transfer_pid_fifo (int dflt_fifo, pid_t secr_pid)
{
    if (write (dflt_fifo, &secr_pid, sizeof(pid_t)) < 0)
    {
        fprintf (stderr, "ERROR! Something wrong with write()\n");
        exit (EXIT_FAILURE);        
    }
}