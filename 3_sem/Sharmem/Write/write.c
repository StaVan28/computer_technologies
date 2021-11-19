#include "write.h"

//---------------------------------------------------------------------

void writer (const char* file_path)
{
    int   id_sem  = create_sem ();

    int   id_shm  = create_shm ();
    char* shmaddr =   link_shm (id_shm); 

    char* tmp_buf = create_tmp_buf ();

    my_semop (id_sem, init_write, 1);
    my_semop (id_sem, sync_read,  2);

    int fd_file = my_open (file_path);

    DBG_PRINT ("\nstart cycle\n");
    int indx = 1;

    int num_symb = -1;
    do
    {
        DBG_PRINT  ("\n");
        PRINT_STEP (indx, %d);
        indx++;

        PRINT_STEP       (p_empty, %p);
        my_semop (id_sem, p_empty, 1);

        PRINT_STEP       (p_mutex, %p);
        my_semop (id_sem, p_mutex, 1);

        if ((num_symb = read (fd_file, tmp_buf, PAGE_SIZE - sizeof (size_t*))) < 0)
        {
            ERROR_INFO ("read ()");
            exit       (EXIT_FAILURE);
        }

        PRINT_STEP (num_symb, %d);

        memcpy (shmaddr + sizeof (size_t*), tmp_buf , PAGE_SIZE - sizeof (size_t*));
        *(size_t*) shmaddr = num_symb;

        memset (tmp_buf, '\0', PAGE_SIZE);

        PRINT_STEP       (v_mutex, %p);
        my_semop (id_sem, v_mutex, 1);

        PRINT_STEP       (v_full, %p);
        my_semop (id_sem, v_full, 1);
    } 
    while (num_symb > 0);

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