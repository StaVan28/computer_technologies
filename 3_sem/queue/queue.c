#include "queue.h"

//------------------------------------------------------------

int create_msg (void)
{
    key_t key = 0;
    if  ((key = ftok (DFLT_PATH, DFLT_SEED)) < 0)
    {
        perror ("ERROR! ftok ()");
        exit   (EXIT_FAILURE);
    }

    int  id = 0;
    if ((id = msgget (key, IPC_CREAT | DFLT_MSG_MODE)) < 0) 
    {
        perror ("ERROR! msgget ()");
        exit   (EXIT_FAILURE);
    }

    return id;
}

//------------------------------------------------------------

void delete_msg (int id)
{
    if (msgctl (id, IPC_RMID, NULL) < 0)
    {
        perror ("ERROR! msgctl ()");
        exit   (EXIT_FAILURE);
    }
}

//------------------------------------------------------------