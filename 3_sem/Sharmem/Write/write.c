#include "write.h"

//---------------------------------------------------------------------

void writer (const char* file_path)
{
    int   id_sem  = create_sem ();

    int   id_shm  = create_shm ();
    char* shmaddr =   link_shm (id_shm);

    char* tmp_buf = create_tmp_buf ();

    struct sembuf alone_wr[2] = {
        {ALONE_WR,  0, 0       },
        {ALONE_WR,  1, SEM_UNDO}
    };
    if (semop (id_sem, alone_wr, 2) < 0)
    {
        ERROR_INFO ("semop ()");
        exit       (EXIT_FAILURE); 
    }

    struct sembuf wait_prev_connect[1] = {
        {NUM_PROC,  0, 0}
    };
    if (semop (id_sem, wait_prev_connect, 1) < 0)
    {
        ERROR_INFO ("semop ()");
        exit       (EXIT_FAILURE); 
    }

    if (semctl (id_sem, FULL, SETVAL, 0) < 0)
    {
        ERROR_INFO ("semop ()");
        exit       (EXIT_FAILURE); 
    }

    struct sembuf sync_read[1] = {
        {SYNC_RD,  1, 0}
    };
    if (semop (id_sem, sync_read, 1) < 0)
    {
        ERROR_INFO ("semop ()");
        exit       (EXIT_FAILURE); 
    }

    struct sembuf sync_write[4] = {
        {SYNC_WR,  -1, 0       },
        {NUM_PROC,  1, SEM_UNDO},
        {FULL,      1, 0       },
        {FULL,     -1, SEM_UNDO}
    };
    if (semop (id_sem, sync_write, 4) < 0)
    {
        ERROR_INFO ("semop ()");
        exit       (EXIT_FAILURE); 
    }

    int  fd_file = -1;
    if ((fd_file = open (file_path, O_RDONLY)) < 0)
    {     
        ERROR_INFO ("open ()");
        exit       (EXIT_FAILURE);
    }

    DBG_PRINT ("\nstart cycle\n");
    int indx = 1;

    int num_symb = -1;
    do
    {
        DBG_PRINT  ("\n");
        PRINT_STEP (indx, %d);
        indx++;
        
        struct sembuf p_empty[1] = {
            {EMPTY, -1, 0}
        };
        if (semop (id_sem, p_empty, 1) < 0)
        {
            ERROR_INFO ("semop ()");
            exit       (EXIT_FAILURE); 
        }

        struct sembuf check_num_proc[2] = {
            {NUM_PROC, -2, IPC_NOWAIT},
            {NUM_PROC,  2, 0         }
        };
        if (semop (id_sem, check_num_proc, 2) < 0)
        {
            ERROR_INFO ("semop ()");
            exit       (EXIT_FAILURE);     
        }

        if ((num_symb = read (fd_file, tmp_buf, PAGE_SIZE - sizeof (size_t*))) < 0)
        {
            ERROR_INFO ("read ()");
            exit       (EXIT_FAILURE);
        }

        PRINT_STEP (num_symb, %d);

        memcpy (shmaddr + sizeof (size_t*), tmp_buf , PAGE_SIZE - sizeof (size_t*));
        *(size_t*) shmaddr = num_symb;

        memset (tmp_buf, '\0', PAGE_SIZE);

        struct sembuf v_full[1] = {
            {FULL, 1, 0}
        };
        if (semop (id_sem, v_full, 1) < 0)
        {
            ERROR_INFO ("semop ()");
            exit       (EXIT_FAILURE);
        }
    } 
    while (num_symb > 0);

    free (tmp_buf);
    if (close (fd_file) < 0)
    {
        ERROR_INFO ("close ()");
        exit       (EXIT_FAILURE);
    }

    struct sembuf end_wr[4] = {
        {FULL,      1, SEM_UNDO},
        {FULL,     -1, 0       },
        {NUM_PROC, -1, SEM_UNDO},
        {ALONE_WR, -1, SEM_UNDO}
    };
    if (semop (id_sem, end_wr, 4) < 0)
    {
        ERROR_INFO ("semop ()");
        exit       (EXIT_FAILURE);     
    }

    unlink_shm (shmaddr);
    delete_shm (id_shm);

    delete_sem (id_sem);
}

//---------------------------------------------------------------------