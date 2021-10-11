#include "fifo_config.h"
//---------------------------------------------------

bool is_can_read_fifo (int secr_fd_fifo);

//---------------------------------------------------

int main (int argc, char const *argv[])
{
    //! CREATE AND CONNECT WITH SECR FIFO

    if (mkfifo (SECR_FIFO, DFLT_MODE) != 0 && errno != EEXIST)
    {
        fprintf (stderr, "ERROR! Smth with mkfifo()\n");
        exit (EXIT_FAILURE);
    }

    errno = 0;

    int  secr_fifo = 0;
    if ((secr_fifo = open (SECR_FIFO, O_RDONLY | O_NONBLOCK)) < 0)
    {
        fprintf (stderr, "ERROR! Smth with open()\n");
        exit (EXIT_FAILURE);
    }
    
    //! CREATE AND CONNECT WITH DFLT FIFO

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
    tv_fifo.tv_sec  = 5;
    tv_fifo.tv_usec = 0;

    int ret_select = select (num_fd, &read_fds, NULL, NULL, &tv_fifo);
    if (ret_select <= 0)
        return false;
    else
        return true;
}