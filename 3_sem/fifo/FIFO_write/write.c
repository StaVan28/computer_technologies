#include "write.h"

//---------------------------------------------------------------------

void write_fifo (const char* file_path)
{
    int dflt_fifo = synchr_fifo (DFLT_FIFO_PATH, O_RDONLY);

    pid_t       secr_pid  = get_pid_fifo (dflt_fifo);
    const char* secr_name = create_name  (secr_pid);
    
    int  secr_fifo = 0;    
    if ((secr_fifo = open (secr_name, O_WRONLY | O_NONBLOCK)) < 0)
    {
        fprintf (stderr, "ERROR! Smth error with open()_1\n");
        exit (EXIT_FAILURE);     
    }

    int  rd_file = 0;    
    if ((rd_file = open (file_path, O_RDONLY)) < 0)
    {
        fprintf (stderr, "ERROR! Smth error with open()\n");
        exit (EXIT_FAILURE);
    }

    data_writing_fifo (rd_file, secr_fifo);

    close (dflt_fifo);
    close (rd_file);
    close (secr_fifo);
}

//---------------------------------------------------------------------

pid_t get_pid_fifo (int dflt_fifo)
{
    pid_t pid;

    if (read (dflt_fifo, &pid, sizeof (pid_t)) < 0)
    {
        fprintf (stderr, "ERROR! Something wrong with read()\n");
        exit (EXIT_FAILURE);        
    }

    return pid;
}

//---------------------------------------------------------------------