#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//-------------------------------------------------------------

const int STDIN_ARGS = 2; 

//

#define BUFF_SIZE 4096

//-------------------------------------------------------------

int main(const int argc, char const *argv[])
{
	if (argc != STDIN_ARGS)
	{
		fprintf(stderr, "ERROR! \nExecute: ./a_out <file> \n");
		return EXIT_FAILURE;
	}
 
	pid_t pid;
	int   fd[2]           = {};
	char  buff[BUFF_SIZE] = {};
	int   num_symb        = 0;

	if (pipe(fd) < 0)
	{
		fprintf(stderr, "ERROR! Smth wrong with pipe()\n");
		exit(EXIT_FAILURE);
	}		

	if ((pid = fork()) < 0)
	{
		fprintf(stderr, "ERROR! Smth wrong with fork()\n");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		waitpid(pid, NULL, 0);

		close(fd[1]);

		while ((num_symb = read(fd[0], buff, BUFF_SIZE)) > 0)
		{
			if (write(STDOUT_FILENO, buff, num_symb) != num_symb)
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

		exit(0);
	}
	else
	{
		close(fd[0]);

		errno = 0;
		int out_file = open(argv[1], O_RDONLY);
		if (errno != 0)
		{
			fprintf(stderr, "ERROR! open() out_file\n");
			exit(EXIT_FAILURE);		
		}

		while ((num_symb = read(out_file, buff, BUFF_SIZE)) > 0)
		{
			if (write(fd[1], buff, num_symb) != num_symb)
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

		exit(0);
	}
	
	exit(0);
}

//-------------------------------------------------------------
