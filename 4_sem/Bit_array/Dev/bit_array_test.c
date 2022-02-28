
#include <stdio.h>
#include <stdlib.h>
#include "bit_array.h"

//-----------------------------------------

static const int NUM_ARGC = 1;

//-----------------------------------------

int main (int argc, const char* argv[])
{
	if (argc != NUM_ARGC)
	{
		fprintf (stderr, "ERROR! Usage ./bit_array_test\n");
		exit    (EXIT_FAILURE);
	}

	int a = 28;
	test_func (a);

	a = 27;
	test_func (a);

	a = -1;
	test_func (a);
}

//-----------------------------------------