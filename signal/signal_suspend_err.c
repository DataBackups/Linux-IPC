#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void handler(int sig)    
{
	printf("SIGINT sig");
}
int main()
{
	sigset_t new,old;
	struct sigaction act;
	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, 0);
	sigemptyset(&new);
	sigaddset(&new, SIGINT);
	sigprocmask(SIG_BLOCK, &new, &old); 
	printf("Blocked");
	sigprocmask(SIG_SETMASK, &old, NULL);
	pause();
	return 0;
}
/*
*上面实例的问题是：本来期望pause()之后，来SIGINT信号，可以结束程序；可是，如果当“取消阻塞”和“pause”之间，正好来了SIGINT信号，结果程序因为pause的原因会一直挂起。。。
*/