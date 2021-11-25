#include "read.h"

//---------------------------------------------------------------------

void reader (void)
{
    int   id_sem  = create_sem ();

    int   id_shm  = create_shm ();
    char* shmaddr =   link_shm (id_shm);

    struct sembuf alone_rd[2] = {
        {ALONE_RD,  0, 0},
        {ALONE_RD,  1, SEM_UNDO}
    };

    my_semop (id_sem, alone_rd, 2);

    struct sembuf init_read [1] = {SYNC_WR,  1, SEM_UNDO};
    struct sembuf sync_write[3] = {
        {SYNC_RD, -1, SEM_UNDO},
        {EMPTY,    1, SEM_UNDO},
        {MUTEX,    1, SEM_UNDO}
    };

    my_semop (id_sem, init_read , 1);
    my_semop (id_sem, sync_write, 3);

    DBG_PRINT ("\nstart cycle\n");
    int indx = 1;

    int num_symb = -1;
    do
    {
        DBG_PRINT  ("\n");
        PRINT_STEP (indx, %d);
        indx++;

        PRINT_STEP       (p_full, %p);
        my_semop (id_sem, p_full, 1);

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

// крит секции
// борется ридер с врайтером за share memory 