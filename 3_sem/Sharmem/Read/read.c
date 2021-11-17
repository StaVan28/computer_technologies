#include "read.h"

//---------------------------------------------------------------------

void reader (void)
{
    int   id_sem  = create_sem ();

    int   id_shm  = create_shm ();
    char* shmaddr =   link_shm (id_shm);

    my_semop (id_sem, init     , 3);
    my_semop (id_sem, sync_read, 1);

    DBG_PRINT ("\nstart cycle\n");
    int indx = 1;

    int num_symb = -1;
    do
    {
        fprintf    (stderr, "\n");
        PRINT_STEP (indx, %d);
        indx++;      

        PRINT_STEP       (p_full, %p);
        my_semop (id_sem, p_full , 1);

        PRINT_STEP       (p_mutex, %p);
        my_semop (id_sem, p_mutex, 1);

        num_symb = *(size_t*) shmaddr;

        if (write (STDOUT_FILENO, shmaddr + sizeof (size_t*), num_symb) != num_symb)
        {
            ERROR_INFO ("write ()");
            exit       (EXIT_FAILURE);
        }

        PRINT_STEP (num_symb, %d);

        PRINT_STEP       (v_mutex, %p);
        my_semop (id_sem, v_mutex, 1);

        PRINT_STEP       (v_empty, %p);
        my_semop (id_sem, v_empty, 1);

    } 
    while (num_symb > 0);

    unlink_shm (shmaddr);
}

//---------------------------------------------------------------------