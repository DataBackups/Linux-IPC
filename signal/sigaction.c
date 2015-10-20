#include <signal.h>
#include <stdio.h>
#include <unistd.h>

//int sigaction(int signum, const struct sigaction *act,
//							struct sigaction *oldact);

//struct sigaction {
//	void     (*sa_handler)(int);
//	void     (*sa_sigaction)(int, siginfo_t *, void *);
//	sigset_t   sa_mask;
//	int        sa_flags;
//	void     (*sa_restorer)(void);
//};


//int sigemptyset(sigset_t *set);
//int sigfillset(sigset_t *set);
//int sigaddset(sigset_t *set, int signum);
//int sigdelset(sigset_t *set, int signum);
//int sigismember(const sigset_t *set, int signum);


void new_op(int signum, siginfo_t *info, void *myact)
{
	printf("receive signal %d\n", signum);
	sleep(5);
}

int main(int argc, char **argv)
{
	struct sigaction act;
	int signum;
	int ret;

	signum = atoi(argv[1]);
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = new_op;

	ret = sigaction(signum, &act, NULL);
	if (ret < 0)
		printf("install signal fail\n");

	while (1) {
		sleep(2);
		printf("wait for the signal\n");
	}

	return 0;
}
/*
*命令行参数为信号值，后台运行./a.out signo &,可获得该进程的ID，假设为PID
*然后再另一个终端上运行kill -s signo pid 进行验证
*
*一个终端
*slzsource/ddu/LinuxIPC/signal# ./a.out  10
*wait for the signal
*wait for the signal
*wait for the signal
*receive signal 10
*wait for the signal
*
*
*另一个终端
*# kill -s 10 13361 
*/