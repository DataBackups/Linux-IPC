#include <unistd.h>
#include <signal.h>
#include <stdio.h>
void handler(int sig)
{
	if(sig == SIGINT)
		printf("SIGINT sig\n");
	else if(sig == SIGQUIT)
		printf("SIGQUIT sig\n");
	else
		printf("SIGUSR1 sig\n");
}

int main()
{
	sigset_t new,old,wait;
	struct sigaction act;
	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, 0);//可以捕捉以下三个信号：SIGINT/SIGQUIT/SIGUSR1
	sigaction(SIGQUIT, &act, 0);
	sigaction(SIGUSR1, &act, 0);

	sigemptyset(&new);
	sigaddset(&new, SIGINT);
	sigemptyset(&wait);
	sigaddset(&wait, SIGUSR1);
	sigprocmask(SIG_BLOCK, &new, &old);

	//临界区代码执行  

	if(sigsuspend(&wait) != -1)//程序在此处挂起；用wait信号集替换new信号集。即：
															//过来SIGUSR1信号，阻塞掉，程序继续挂起；过来其他信号，
															//例如SIGINT，则会唤醒程序。执行sigsuspend的原子操作。
															//注意：如果“sigaddset(&wait, SIGUSR1);”这句没有，
															//则此处不会阻塞任何信号，即过来任何信号均会唤醒程序。
		printf("sigsuspend error\n");
	printf("After sigsuspend\n");
	sigprocmask(SIG_SETMASK, &old, NULL);
	return 0;
}
