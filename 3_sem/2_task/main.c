#include <stdio.h>
#include <stdlib.h>		
#include <fcntl.h>
#include <errno.h>

//-------------------------------------------------------------

const int    STDIN_ARGS = 3;
const mode_t  MODE_OPEN = S_IRUSR | S_IWUSR	;

//!

#define BUFF_SIZE 4096

//-------------------------------------------------------------

void my_cp(const char* out_file_path, const char* in_file_path);

//-------------------------------------------------------------

int main(int argc, const char *argv[])
{
	if (argc != STDIN_ARGS)
	{
		fprintf(stderr, "ERROR! \nExecute: ./a_out <out_file> <in_file>\n");
		return EXIT_FAILURE;
	}

	my_cp(argv[1], argv[2]);

	return 0;
}

//-------------------------------------------------------------

void my_cp(const char* out_file_path, const char* in_file_path)
{
	int  num_symb        = 0;
	char buff[BUFF_SIZE] = {};

	int out_fd = open(out_file_path, O_RDONLY);
	int  in_fd = open( in_file_path, O_WRONLY | O_CREAT, MODE_OPEN);

	while ((num_symb = read(out_fd, buff, BUFF_SIZE)) > 0)
	{
		if (write(in_fd, buff, num_symb) != num_symb)
		{
			fprintf(stderr, "ERROR! Something wrong with write()\n");
			exit(EXIT_FAILURE);
		}
	}

	if (num_symb < 0)
	{
		fprintf(stderr, "ERROR! Something wrong with read()\n");
		exit(EXIT_FAILURE);		
	}	
}

//-------------------------------------------------------------