#include "write.h"

//---------------------------------------------------------------------

static const int STDIN_ARGS = 2;

//---------------------------------------------------------------------

int main (const int argc, const char *argv[])
{
    if (argc != STDIN_ARGS)
    {
        perror ("ERROR! Execute: ./a_out <out_file>\n");
        return EXIT_FAILURE;
    }

    writer (argv[1]);

    return 0;
}

//---------------------------------------------------------------------