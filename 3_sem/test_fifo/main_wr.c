#include "fifo_config.h"

//---------------------------------------------------

pid_t get_pid_fifo(int dflt_fifo);

//---------------------------------------------------

int main (int argc, char const *argv[])
{
    assert (argc == 2);
    
    //! CREATE AND CONNECT WITH DFLT FIFO

    if (mkfifo (DFLT_FIFO, DFLT_MODE) != 0 && errno != EEXIST)
    {
        fprintf (stderr, "ERROR! Smth with mkfifo()\n");
        exit (EXIT_FAILURE);
    }

    errno = 0;

    int  dflt_fifo = 0;
    if ((dflt_fifo = open (DFLT_FIFO, O_RDONLY)) < 0)
    {
        fprintf (stderr, "ERROR! Smth with open()_dflt\n");
        exit (EXIT_FAILURE);
    }

    pid_t       secr_pid  = get_pid_fifo(dflt_fifo);
    const char* secr_name = create_name (secr_pid);

    //! CONNECT WITH SECR FIFO

    int  secr_fifo = 0;
    if ((secr_fifo = open (secr_name, O_WRONLY)) < 0)
    {
        fprintf (stderr, "ERROR! Smth with open()\n");
        exit (EXIT_FAILURE);
    }

    //! OPEN FILE

    int  rd_file = 0;
    if ((rd_file = open (argv[1], O_RDONLY)) < 0)
    {
        fprintf (stderr, "ERROR! Smth with open()\n");
        exit (EXIT_FAILURE);
    }

    //! WRITE TO SECR FIFO

    write_data (rd_file, secr_fifo);

    close(dflt_fifo);
    close(rd_file);
    close(secr_fifo);

    return 0;
}

//---------------------------------------------------

pid_t get_pid_fifo(int dflt_fifo)
{
    pid_t pid;
 
    if (read (dflt_fifo, &pid, sizeof(pid_t)) < 0)
    {
        fprintf (stderr, "ERROR! Something wrong with read()\n");
        exit (EXIT_FAILURE);        
    }

    return pid;
}