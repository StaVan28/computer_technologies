#include "read.h"

//---------------------------------------------------------------------

void reader (void)
{
    int   id_sem  = create_sem ();

    int   id_shm  = create_shm ();
    char* shmaddr =   link_shm (id_shm); 

    PAUSE;

    // print out
    printf("!!!!!%s!!!!!\n", shmaddr);

    unlink_shm (shmaddr);
    delete_shm (id_shm);

    delete_sem (id_sem);
}

//---------------------------------------------------------------------