#include "write.h"

//------------------------------------------------------------

static struct my_error_t error_info = {0};

//---------------------------------------------------------------------

void writer (const char* file_path)
{
    char* shmaddr = create_shm();

    getchar();

    delete_shm (shmaddr);
}

//---------------------------------------------------------------------