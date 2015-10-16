#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	int pipe_fd[2];
	int ret;
	char r_buf[4];
	char w_buf[4];
	pid_t pid;
	int childexit = 0;

	memset(r_buf, 0, sizeof(r_buf));

	ret = pipe(pipe_fd);
	if (ret < 0) {
		printf("pipe fail\n");
		return -1;
	}

	pid = fork();
	if (pid == 0) {
		close(pipe_fd[1]);	
		sleep(2);

		while(!childexit) {
			read(pipe_fd[0], r_buf, 4);
			if (!strcmp("0", r_buf)) {
				printf("\n");
				printf("child: receive command form parent over\n");	
				printf("now child process exit\n");
				childexit = 1;
			}

			sleep(1);
		}

		close(pipe_fd[0]);
		exit(0);
	} else if (pid > 0) {
		close(pipe_fd[0]);		
		strcpy(w_buf, "0");
		ret = write(pipe_fd[1], w_buf, 4);
		if (ret < 0)
			printf("write fail\n");
		close(pipe_fd[1]);
	} else {
		printf("fork fail\n");	
	}
	

	return 0;
}
