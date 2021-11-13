#include "sharmem_config.h"

//------------------------------------------------------------

static struct my_error_t error_info = {0};

//------------------------------------------------------------

char* create_shm (void)
{
    key_t key = -1;
    if  ((key = ftok (DFLT_FTOK_PATH, DFLT_FTOK_SEED)) < 0)
    {
        ERROR_INFO ("ftok ()");
        exit       (EXIT_FAILURE);
    }

    PRINT_STEP (key, %d);

    int  id = -1;
    if ((id = shmget (key, PAGE_SIZE, IPC_CREAT | DFLT_MODE)) < 0) 
    {
        ERROR_INFO ("shmmat ()");
        exit       (EXIT_FAILURE);
    }

    PRINT_STEP (id, %d);

    void* shmaddr = NULL;
    if  ((shmaddr = shmat (id, NULL, 0)) == NULL)
    {
        ERROR_INFO ("shmat ()");
        exit       (EXIT_FAILURE);
    }

    PRINT_STEP (shmaddr, %p);

    return (char*) shmaddr;
}

//---------------------------

void delete_shm (char* shmaddr)
{
    if (shmdt ((void*) shmaddr))
    {
        ERROR_INFO ("shmdt ()");
        exit       (EXIT_FAILURE);        
    }
}

//------------------------------------------------------------