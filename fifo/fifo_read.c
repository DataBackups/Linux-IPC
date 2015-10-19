#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define FIFO_SERVER "fifoserver"

int main()
{
	char r_buf[4096*2];
	int fd;
	int ret;

	memset(r_buf, 0, sizeof(r_buf));

	ret = mkfifo(FIFO_SERVER, O_CREAT);
	if (ret < 0 && errno != EEXIST)
		printf("cannot create fifoserver\n");

	fd = open(FIFO_SERVER, O_RDONLY | O_NONBLOCK, 0);
	if (fd < 0)
		printf("open fail\n");

	while (1) {
		memset(r_buf, 0, sizeof(r_buf));
		ret = read(fd, r_buf, 1024);
		if (ret < 0) {
			printf("read fail\n");
			if (errno == EAGAIN) 
				printf("No data yet\n"); 
		}
		printf("read %d bytes\n", ret);
		sleep(1);
	}

	unlink(FIFO_SERVER);
	return 0;
}
