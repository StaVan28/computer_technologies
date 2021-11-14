#include "read.h"

//---------------------------------------------------------------------

void reader (void)
{
    int   id_sem  = create_sem ();

    int   id_shm  = create_shm ();
    char* shmaddr =   link_shm (id_shm);

    char* tmp_buf = create_tmp_buf ();

    my_semop (id_sem, init     , 3);
    my_semop (id_sem, sync_read, 1);

    DBG_PRINT ("\nstart cycle\n");
    int indx = 1;

    do
    {
        fprintf    (stderr, "\n");
        PRINT_STEP (indx, %d);
        indx++;      

        PRINT_STEP       (p_full, %p);
        my_semop (id_sem, p_full , 1);

        PRINT_STEP       (p_mutex, %p);
        my_semop (id_sem, p_mutex, 1);

        memcpy  (tmp_buf, shmaddr, PAGE_SIZE - 1);
        tmp_buf [PAGE_SIZE - 1] = '\0';
        
        int ret_fprintf = fprintf (stdout, "%s", tmp_buf);
        fflush  (stdout);

        PRINT_STEP       (v_mutex, %p);
        my_semop (id_sem, v_mutex, 1);

        PRINT_STEP       (v_empty, %p);
        my_semop (id_sem, v_empty, 1);

        PRINT_STEP (ret_fprintf, %d);
    } 
    while (tmp_buf [PAGE_SIZE - 2] != '\0');

    unlink_shm (shmaddr);
}

//---------------------------------------------------------------------