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
*����ʵ���������ǣ���������pause()֮����SIGINT�źţ����Խ������򣻿��ǣ��������ȡ���������͡�pause��֮�䣬��������SIGINT�źţ����������Ϊpause��ԭ���һֱ���𡣡���
*/