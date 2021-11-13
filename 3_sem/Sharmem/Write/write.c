#include "write.h"

//---------------------------------------------------------------------

void writer (const char* file_path)
{
    int   id_sem  = create_sem ();

    int   id_shm  = create_shm ();
    char* shmaddr =   link_shm (id_shm); 

    int   fd_file = my_open        (file_path);
    char* tmp_buf = create_tmp_buf ();

    while (true)
    {
        break;
    }

    // print in
    strncpy (shmaddr, "hello", PAGE_SIZE);

    PAUSE;

    free       (tmp_buf);
    my_close   (fd_file);

    unlink_shm (shmaddr);
    delete_shm (id_shm);

    delete_sem (id_sem);
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

//---------------------------------------------------------------------

char* create_tmp_buf (void)
{
    DBG_PRINT ("\ncreate_tmp_buf ()\n");

    char* tmp_buf = NULL;
    if  ((tmp_buf = (char*) calloc (1, PAGE_SIZE)) == NULL)
    {
        ERROR_INFO ("calloc ()");
        exit       (EXIT_FAILURE);
    }

    return tmp_buf;
}