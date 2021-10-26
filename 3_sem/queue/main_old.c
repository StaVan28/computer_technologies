#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

//-------------------------------------------------------------

const int NUM_PARSING = 2; 

//-------------------------------------------------------------

char const* parsing_input(const int argc, char const *argv[]);

long my_strtol(char const *digit_str, char *end_digit_str);

//-------------------------------------------------------------

int main(const int argc, char const *argv[])
{
	char const     *digit_str = parsing_input(argc, argv);  
	char       *end_digit_str = NULL; 

	pid_t pid;

	long inp_digit = my_strtol(digit_str, end_digit_str);

	for (int i = 0; i < inp_digit; i++)
	{
		if ((pid = fork()) < 0)
		{
			fprintf(stderr, "ERROR! Smth wrong with fork()\n");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			printf("Serial number = %d, "
				   "Child pid     = %d, "
			   	   "Parent pid    = %d\n", i + 1, getpid(pid), getppid(pid));

			exit(0);
		}
	}
	
	return 0;
}

//-------------------------------------------------------------

char const* parsing_input(const int argc, char const *argv[])
{
	if (argc != NUM_PARSING)
	{
		fprintf(stderr, "ERROR! \nExecute: ./a.out <number>\n");
		exit(EXIT_FAILURE);
	}

	return argv[NUM_PARSING - 1];
}

//-------------------------------------------------------------

long my_strtol(char const *digit_str, char *end_digit_str)
{
	errno = 0;

	long inp_digit = strtol(digit_str, &end_digit_str, 10);

	if (errno != 0)
	{
		fprintf(stderr, "ERROR! strtol(): smth went wrong...\n");
		exit(EXIT_FAILURE);
	}
	if (*end_digit_str != '\0')
	{
		fprintf(stderr, "ERROR! strtol(): not a number...\n");
		exit(EXIT_FAILURE);
	}
	if (inp_digit < 1)
	{
		fprintf(stderr, "ERROR! Please, input natural number\n");
		exit(EXIT_FAILURE);
	}

	return inp_digit;	
}

