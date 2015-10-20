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
