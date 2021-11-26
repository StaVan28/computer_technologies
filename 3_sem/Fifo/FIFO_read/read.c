#include "read.h"

//---------------------------------------------------------------------

static const int WAIT_TIME = 5;

//---------------------------------------------------------------------

void read_fifo (void)
{
    pid_t       secr_pid  = getpid      ();
    const char* secr_name = create_name (secr_pid);

    int secr_fifo = synchr_fifo (secr_name     , O_RDONLY | O_NONBLOCK);

    if (fcntl (secr_fifo, F_SETFL, O_RDONLY) < 0)
    {
        perror ("ERROR! Smth wrong with fcntl()!\n");
        exit   (EXIT_FAILURE);
    }

    int dflt_fifo = synchr_fifo (DFLT_FIFO_PATH, O_WRONLY);

    transfer_pid_fifo (dflt_fifo, secr_pid);

    if (!is_can_read_fifo (secr_fifo))
    {
        perror ("ERROR! Time out!\n");
        exit   (EXIT_FAILURE);
    }

    data_writing_fifo (secr_fifo, STDOUT_FILENO);

    close (secr_fifo);
    close (dflt_fifo);
}

//---------------------------------------------------------------------

void transfer_pid_fifo (int dflt_fifo, pid_t secr_pid)
{
    if (write (dflt_fifo, &secr_pid, sizeof (pid_t)) < 0)
    {
        perror ("ERROR! Something wrong with write()\n");
        exit   (EXIT_FAILURE);
    }
}

//---------------------------------------------------------------------

bool is_can_read_fifo (int secr_fifo)
{
    int num_fd = secr_fifo + 1;

    fd_set read_fds = {};
    FD_ZERO (           &read_fds);
    FD_SET  (secr_fifo, &read_fds);

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