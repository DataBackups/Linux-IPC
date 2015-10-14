#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//int pipe(int pipefd[2]);

int main()
{
	int pipe_fd[2];
	int ret = 0;
	pid_t pid;
	char r_buf[100];
	char w_buf[4];
	int i;

	memset(r_buf, 0, sizeof(r_buf));
	memset(w_buf, 0, sizeof(w_buf));

	ret = pipe(pipe_fd);
	if (ret < 0) {
		printf("pipe create error\n");
		return -1;
	}

	pid = fork();
	if (pid == 0) {
		close(pipe_fd[1]);	
		sleep(3); //wait father close pipe
		ret	= read(pipe_fd[0], r_buf, 100);
		if (ret < 0)
			printf("read failed\n");
		printf("read num is %d\n", ret);
		printf("the data read form the pipe is ");
		for (i=0; i<ret; i++)
			printf("%c", r_buf[i]);

		printf("\n");
		close(pipe_fd[0]);
		exit(0);
	} else if (pid > 0) {	
		close(pipe_fd[0]);
		strcpy(w_buf, "111");
		ret = write(pipe_fd[1], w_buf, 4);
		if (ret < 0)
			printf("write failed\n");
		printf("parent write over\n");
		
		close(pipe_fd[1]);
		printf("parent close fd[1] over\n");

		sleep(10);
	} else {
		printf("fork failed\n");
	}

	return 0;
}
