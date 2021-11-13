#include "write.h"

//---------------------------------------------------------------------

void writer (const char* file_path)
{
    char* shmaddr = create_shm();

    int   fd_file = my_open (file_path);

    strncpy (shmaddr, "hello", PAGE_SIZE);

    PAUSE;

    my_close   (fd_file);

    delete_shm (shmaddr);
}

//---------------------------------------------------------------------

int my_open (const char* file_path)
{
    DBG_PRINT ("\nopen ()\n");

    int  fd_file = -1;
    if ((fd_file = open (file_path, O_RDONLY)) < 0)
    {
        ERROR_INFO ("open ()");
        exit       (EXIT_FAILURE);
    }

    return fd_file;
}

//---------------------------------------------------------------------

void my_close (int fd_file)
{
    DBG_PRINT ("\nclose ()\n");

    if (close (fd_file) < 0)
    {
        ERROR_INFO ("close ()");
        exit       (EXIT_FAILURE);
    }
}