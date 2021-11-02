#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

int fd_test (int fd_1, int fd_2);

int main (int argc, const char *argv[])
{
	const char* file = "/run/media/stavan/PASSCAPE/test_file";

	int fd1 = open (file, O_RDONLY);
	printf ("fd1 = %x\n", fd1);

	printf ("Pause START\n");

	getchar ();

	printf ("Pause END\n");

	int fd2 = open (file, O_WRONLY);
	printf ("fd2 = %x\n", fd2);

	printf ("fd1 = %d, fd2 = %d\n", fd1, fd2);
	printf ("RESULT TEST = %d\n"  , fd_test (fd1, fd2));

	return 0;
}

int fd_test (int fd_1, int fd_2)
{
	struct stat buf_1 = {};
	struct stat buf_2 = {};

	if (fstat (fd_1, &buf_1) < 0)
	{
		perror ("ERROR! fstat");
		exit   (EXIT_FAILURE);
	}

	if (fstat (fd_2, &buf_2) < 0)
	{
		perror ("ERROR! fstat");
		exit   (EXIT_FAILURE);
	}

	printf ("buf_1.st_dev = %x, buf_1.st_ino = %x, buf_1.st_size = %x\n"
		    "buf_2.st_dev = %x, buf_2.st_ino = %x, buf_2.st_size = %x\n",
		     buf_1.st_dev     , buf_1.st_ino     , buf_1.st_size, 
		     buf_2.st_dev     , buf_2.st_ino     , buf_2.st_size         );

	if (buf_1.st_dev == buf_2.st_dev && 
		buf_1.st_ino == buf_2.st_ino   )
		return 1;
	else
		return 0;
}

/*

1) подмена во время getchar()
2) смена hardlink
3) смена softlink
4) подмена папки (./test_dir_1/file ==> ./test_dir_2/file)
5) dismount/amount флешки

*/