#include "select.h"

//----------------------------------------------

static const int STDIN_ARGC = 3;

void transfer_data (int num_proc, const char* file_name);

//----------------------------------------------

int main(int argc, const char *argv[])
{
	if (argc != STDIN_ARGC)
	{
        fprintf (stderr, "ERROR! Execute: ./a_out <num_proc> <out_file>\n");
        return EXIT_FAILURE;
	}

	int num_proc = get_num (argc, argv);

	transfer_data (num_proc, argv[2]);
	
	return 0;
}

//----------------------------------------------
