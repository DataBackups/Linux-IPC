#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	int pipe_fd[2];
	pid_t pid;
	int ret = 0;
	char r_buf[4];
	char w_buf[4];

	ret = pipe(pipe_fd);
	if (ret < 0)
		printf("pipe fail\n");

	memset(r_buf, 0, sizeof(r_buf));
	memset(w_buf, 0, sizeof(w_buf));

	pid = fork();
	if (pid == 0) {
		close(pipe_fd[0]);	
		close(pipe_fd[1]);
		sleep(10);
		exit(0);
	} else if (pid > 0) {
		sleep(1); //wait child close pipe	
		close(pipe_fd[0]);
		strcpy(w_buf, "111");
		ret = write(pipe_fd[1], w_buf, 4);
		if (ret < 0)
			printf("write fail\n");
		else
			printf("the bytes write to pipe is %d\n", ret);
		close(pipe_fd[1]);
	} else {
		printf("fork fail\n");
	}

	return 0;
}
/*
*对管道写规则验证：写端对读端存在依赖性
*只用在管道的读端存在时，向管道中写入数据才有意义，否则向管道中写入数据的进程将收到SIFPIPE信号，应用程序可以处理该信号，也可以忽略（默认动作是应用程序终止）
*/
