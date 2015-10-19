#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

//int mkfifo(const char *pathname, mode_t mode);


#define FIFO_SERVER "fifoserver"

int main()
{
	int fd;
	char w_buf[4096*2];
	int ret;

	memset(w_buf, 0, 4096*2);

	fd = open(FIFO_SERVER, O_WRONLY|O_NONBLOCK, 0);
	if (fd < 0)
		printf("open error\n");

	ret = write(fd, w_buf, 4096);
	if (ret < 0) {
		printf("write fail\n");
		if (errno == EAGAIN) 
			printf("The FIFO has not been read yet.Please try later\n");
	}

	return 0;
}
