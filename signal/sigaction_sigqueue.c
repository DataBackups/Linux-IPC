#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


//int sigaction(int signum, const struct sigaction *act,
//                          struct sigaction *oldact);

//struct sigaction {
//  void     (*sa_handler)(int);
//  void     (*sa_sigaction)(int, siginfo_t *, void *);
//  sigset_t   sa_mask;
//  int        sa_flags;
//  void     (*sa_restorer)(void);
//};


//int sigemptyset(sigset_t *set);
//int sigfillset(sigset_t *set);
//int sigaddset(sigset_t *set, int signum);
//int sigdelset(sigset_t *set, int signum);
//int sigismember(const sigset_t *set, int signum);

//int sigqueue(pid_t pid, int sig, const union sigval value);

//union sigval {
//	int   sival_int;
//	void *sival_ptr;
//};

void new_op(int signum, siginfo_t *info, void *myact)
{
	int i;

	printf("handle signal %d\n", signum);
	for (i=0; i<10; i++)
		//printf("%c ", (*((char *)((*info).si_ptr) + i)));
		printf("%c ", (*((char *)(info->si_ptr + i))));
	printf("\n");

}

int main(int argc, char **argv)
{
	int signum;
	struct sigaction act;
	union sigval value;
	int i;
	pid_t pid;
	char data[16] = {'T', 'h', 'a', 'n', 'k', ' ', 'y', 'o', 'u', '.'};
	int ret;

	pid = getpid();

	value.sival_ptr = data;
	signum = atoi(argv[1]);//没有对错误进行处理
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;//SA_SIGINFO 是否传递参数的开关
	act.sa_sigaction = new_op;

	ret = sigaction(signum, &act, NULL);
	if (ret < 0)
		printf("install signal error\n");

	while (1) {
		sleep(2);
		printf("wait for the signal\n");
		sigqueue(pid, signum, value); //发送信号
	}

	return 0;
}
/*
*执行./a.out 35
*slzsource/ddu/LinuxIPC/signal# ./a.out 35
*wait for the signal
*handle signal 35
*T h a n k   y o u . 
*wait for the signal
*handle signal 35
*T h a n k   y o u . 
*/