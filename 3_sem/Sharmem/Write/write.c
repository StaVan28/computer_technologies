#include "write.h"

//---------------------------------------------------------------------

void writer (const char* file_path)
{
    int   id_sem  = create_sem ();

    int   id_shm  = create_shm ();
    char* shmaddr =   link_shm (id_shm); 

    int   fd_file = my_open        (file_path);
    char* tmp_buf = create_tmp_buf ();

    DBG_PRINT ("\nstart cycle\n");
    int indx = 1;

    int    num_symb = 1;
    while (num_symb > 0)
    {
        PRINT_STEP (indx, %d);
        indx++;

        if ((num_symb = read (fd_file, tmp_buf, PAGE_SIZE - 1)) < 0)
        {
            ERROR_INFO ("read ()");
            exit       (EXIT_FAILURE);
        }

        PRINT_STEP (num_symb, %d);

        memcpy  (shmaddr, tmp_buf, PAGE_SIZE - 1);
        shmaddr [PAGE_SIZE - 1] = '\0';
        memset  (tmp_buf, '\0', PAGE_SIZE);

        PAUSE;
    }

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