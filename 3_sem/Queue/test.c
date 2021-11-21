#include <stdio.h>
#include <assert.h>

int main (int argc, const char *argv[])
{
	FILE* test = fopen ("./test.txt", "wb");
	assert (test);

	for (int i = 1; i < 8000 + 1; i++)
	{
		fprintf (test, "Child proc = %d\n", i);
	}

	fclose (test);
}