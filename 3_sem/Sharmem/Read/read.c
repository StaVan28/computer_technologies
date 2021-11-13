#include "read.h"

//---------------------------------------------------------------------

void reader (void)
{
    char* shmaddr = create_shm();

    PAUSE;

    printf("!!!!!%s!!!!!\n", shmaddr);

    delete_shm (shmaddr);
}

//---------------------------------------------------------------------