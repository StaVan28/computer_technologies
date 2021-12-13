#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

int main()
{
	char buf[5];
	mkfifo("fifo", 0666);
	int fd = open("fifo", O_RDWR);
	write(fd, "aaa\n", 4);
	fd = open("fifo", O_RDONLY | O_NDELAY);
	write(STDOUT_FILENO, buf, read(fd, buf, 4));
}