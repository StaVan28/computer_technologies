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
    if (semop (id_sem, alone_rd, 2) < 0)
    {
        ERROR_INFO ("semop ()");
        exit       (EXIT_FAILURE); 
    }

    struct sembuf check_num_proc[1] = {
        {NUM_PROC,  0, 0}
    };
    if (semop (id_sem, check_num_proc, 1) < 0)
    {
        ERROR_INFO ("semop ()");
        exit       (EXIT_FAILURE); 
    }

    struct sembuf sync_write_v[1] = {
        {SYNC_WR,  1, 0}
    };
    if (semop (id_sem, sync_write_v, 1) < 0)
    {
        ERROR_INFO ("semop ()");
        exit       (EXIT_FAILURE); 
    }

    struct sembuf sync_read[3] = {
        {SYNC_RD, -1, 0},
        {EMPTY,    1, SEM_UNDO},
        {NUM_PROC, 1, SEM_UNDO}
    };
    if (semop (id_sem, sync_read, 3) < 0)
    {
        ERROR_INFO ("semop ()");
        exit       (EXIT_FAILURE); 
    }

    DBG_PRINT ("\nstart cycle\n");
    int indx = 1;

    struct timespec timeout;
    timeout.tv_sec  = 3;
    timeout.tv_nsec = 0;
    
    int num_symb = -1;
    do
    {
        DBG_PRINT  ("\n");
        PRINT_STEP (indx, %d);
        indx++;

        struct sembuf p_full[1] = {
            {FULL, -1, 0}
        };
        if (semtimedop (id_sem, p_full, 1, &timeout) < 0)
        {
            ERROR_INFO ("semop ()");
            exit       (EXIT_FAILURE); 
        }

        struct sembuf check_num_proc[2] = {
            {NUM_PROC, -2, IPC_NOWAIT},
            {NUM_PROC,  2, 0},
        };
        if (semop (id_sem, check_num_proc, 2) < 0)
        {
            ERROR_INFO ("semop ()");
            exit       (EXIT_FAILURE);     
        }

        num_symb = *(size_t*) shmaddr;

        if (write (STDOUT_FILENO, shmaddr + sizeof (size_t*), num_symb) != num_symb)
        {
            ERROR_INFO ("write ()");
            exit       (EXIT_FAILURE);
        }

        PRINT_STEP (num_symb, %d);

        struct sembuf v_empty[1] = {
            {EMPTY, 1, 0}
        };
        if (semop (id_sem, v_empty, 1) < 0)
        {
            ERROR_INFO ("semop ()");
            exit       (EXIT_FAILURE);
        }
    } 
    while (num_symb > 0);

    errno = 0;

    struct sembuf end_rd[3] = {
        {NUM_PROC, -1, SEM_UNDO},
        {EMPTY,    -1, SEM_UNDO},
        {ALONE_RD, -1, SEM_UNDO}
    };
    if (semop (id_sem, check_num_proc, 3) < 0 && errno != EINVAL)
    {
        ERROR_INFO ("semop ()");
        exit       (EXIT_FAILURE);     
    }

    errno = 0;

    unlink_shm (shmaddr);
}

//---------------------------------------------------------------------

//