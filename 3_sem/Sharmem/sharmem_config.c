#include "sharmem_config.h"

//------------------------------------------------------------

int create_shm (void)
{
    DBG_PRINT ("\ncreate_shm()\n\n");

    key_t key = -1;
    if  ((key = ftok (DFLT_FTOK_PATH, DFLT_FTOK_SEED)) < 0)
    {
        ERROR_INFO ("ftok ()");
        exit       (EXIT_FAILURE);
    }

    PRINT_STEP (key, %d);

    int  id_shm = -1;
    if ((id_shm = shmget (key, PAGE_SIZE, IPC_CREAT | DFLT_MODE)) < 0) 
    {
        ERROR_INFO ("shmget ()");
        exit       (EXIT_FAILURE);
    }

    PRINT_STEP (id_shm, %d);

    return id_shm;
}

//---------------------------

void delete_shm (int id_shm)
{
    DBG_PRINT  ("\ndelete_shm()\n\n");

    PRINT_STEP (id_shm, %d);

    if ((shmctl (id_shm, IPC_RMID, NULL) < 0) && errno == EIDRM)
    {
        ERROR_INFO ("shmctl ()");
        exit       (EXIT_FAILURE);
    }

    errno = 0;
}

//---------------------------

int create_sem (void)
{
    DBG_PRINT ("\ncreate_sem()\n\n");

    key_t key = -1;
    if  ((key = ftok (DFLT_FTOK_PATH, DFLT_FTOK_SEED)) < 0)
    {
        ERROR_INFO ("ftok ()");
        exit       (EXIT_FAILURE);
    }

    PRINT_STEP (key, %d);

    int  id_sem = -1;
    if ((id_sem = semget (key, NUM_SEM, IPC_CREAT | DFLT_MODE)) < 0) 
    {
        ERROR_INFO ("semget ()");
        exit       (EXIT_FAILURE);
    }

    PRINT_STEP (id_sem, %d);

    return id_sem;    
}

//---------------------------

void delete_sem (int id_sem)
{
    DBG_PRINT  ("\ndelete_sem()\n\n");

    PRINT_STEP (id_sem, %d);

    if ((semctl (id_sem, NUM_SEM, IPC_RMID) < 0) && errno == EIDRM)
    {
        ERROR_INFO ("semctl ()");
        exit       (EXIT_FAILURE);
    }

    errno = 0;
}

//---------------------------

char* link_shm (int id_shm)
{
    DBG_PRINT ("\nlink_shm()\n\n");

    void* shmaddr = NULL;
    if  ((shmaddr = shmat (id_shm, NULL, 0)) == NULL)
    {
        ERROR_INFO ("shmat ()");
        exit       (EXIT_FAILURE);
    }

    PRINT_STEP (shmaddr, %p);

    return (char*) shmaddr;
}

//---------------------------

void unlink_shm (char* shmaddr)
{
    DBG_PRINT  ("\ndelete_shm()\n\n");

    PRINT_STEP (shmaddr, %p);

    if (shmdt ((void*) shmaddr))
    {
        ERROR_INFO ("shmdt ()");
        exit       (EXIT_FAILURE);        
    }
}

//----------------------------

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

//----------------------------

void my_semop (int id_sem, struct sembuf *sops, unsigned nsops)
{
    if (semop (id_sem, sops, nsops) < 0)
    {
        ERROR_INFO ("semop ()");
        exit       (EXIT_FAILURE); 
    }
}

//------------------------------------------------------------