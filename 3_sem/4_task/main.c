#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

//-------------------------------------------------------------

const int STDIN_ARGS = 2;

//-------------------------------------------------------------

int main(const int argc, const char *argv[])
{
	if (argc < STDIN_ARGS)
	{
		fprintf(stderr, "ERROR! \nExecute: ./a_out <exec_prog> ...\n");
		exit(EXIT_FAILURE);
	}

	pid_t pid;
	char** execvp_argv = (char**) calloc(argc + 1, sizeof(char*));

	for (int i = 0; i < argc; i++)
	{
		execvp_argv[i] = argv[i + 1];
	}

	if ((pid = fork()) < 0)
	{
		fprintf(stderr, "ERROR! Smth wrong with fork()\n");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		if (execvp(argv[1], execvp_argv))
		{
			fprintf(stderr, "ERROR! Smth wrong with execvp()\n");
			exit(EXIT_FAILURE);
		}

		exit(0);
	}

	exit(0);
}

//-------------------------------------------------------------
	