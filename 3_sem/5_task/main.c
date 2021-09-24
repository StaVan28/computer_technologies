#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

//-------------------------------------------------------------

const      int NUM_PARSING = 2;
const long int GL_INCR     = 10000;

//!
	
long int GL_COUNTER = 0;

//-------------------------------------------------------------

char const* parsing_input(const int argc, char const *argv[]);

long my_strtol(char const *digit_str, char *end_digit_str);

void* incr_gl_counter(void*);

//-------------------------------------------------------------

int main(const int argc, char const *argv[])
{
	char const     *digit_str = parsing_input(argc, argv);  
	char       *end_digit_str = NULL; 

	pthread_t pthread;

	long inp_digit = my_strtol(digit_str, end_digit_str);

	for (int i = 0; i < inp_digit; i++)
	{
		if ((pthread = pthread_create(&pthread, NULL, incr_gl_counter, NULL)) != 0)
		{
			fprintf(stderr, "ERROR! Smth wrong with fork()\n");
			exit(EXIT_FAILURE);
		}
		
	}

	printf("Quantity pthreads = %ld\n"
		   "GLOBAL_COUNTER    = %ld\n"
		   "Wait result       = %ld\n", inp_digit, GL_COUNTER, inp_digit * GL_INCR);
	
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

//----------------------------------------------------------------

void* incr_gl_counter(void*)
{	
	GL_COUNTER += GL_INCR;

	return NULL;
}

