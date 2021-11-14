#include "read.h"

//---------------------------------------------------------------------

void reader (void)
{
    int   id_sem  = create_sem ();

    int   id_shm  = create_shm ();
    char* shmaddr =   link_shm (id_shm);

    char* tmp_buf = create_tmp_buf ();

    FILE* fd_file = NULL;
    if  ((fd_file = fopen ("../test1.txt", "w+")) < 0)
    {     
        ERROR_INFO ("fopen ()");
        exit       (EXIT_FAILURE);
    }

    DBG_PRINT ("\nstart cycle\n");
    int indx = 1;

    do
    {
        PRINT_STEP (indx, %d);
        indx++;      

        memcpy  (tmp_buf, shmaddr, PAGE_SIZE - 1);
        tmp_buf [PAGE_SIZE - 1] = '\0';
        
        int ret_fprintf = fprintf (fd_file, "%s", tmp_buf);
        fflush  (stdout);

        PRINT_STEP (ret_fprintf, %d);

        PAUSE;
    } 
    while (tmp_buf [PAGE_SIZE - 2] != '\0');

    fclose (fd_file);

    unlink_shm (shmaddr);
    delete_shm (id_shm);

    delete_sem (id_sem);
}

//---------------------------------------------------------------------