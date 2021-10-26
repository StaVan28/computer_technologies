#include "config.h"

//------------------------------------------

int parsing_input (const int argc, const char *argv[])
{
	if (argc != STDIN_ARGC)
	{
		fprintf (stderr, "ERROR! Execute: ./a.out <number>\n");
		exit    (EXIT_FAILURE);
	}

	char* end_digit_str = NULL;
	long  inp_digit     = strtol (argv[1], &end_digit_str, DIGIT_BASE);

	if (errno != 0)
	{
		perror ("ERROR! strtol(): smth went wrong...");
		exit   (EXIT_FAILURE);
	}

	if (*end_digit_str != '\0')
	{
		perror ("ERROR! strtol(): not a number...");
		exit   (EXIT_FAILURE);
	}

	if (inp_digit < 1)
	{
		perror ("ERROR! Please, input natural number\n");
		exit   (EXIT_FAILURE);
	}

	return inp_digit;
}