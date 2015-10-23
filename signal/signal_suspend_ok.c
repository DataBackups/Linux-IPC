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
	sigaction(SIGINT, &act, 0);//���Բ�׽���������źţ�SIGINT/SIGQUIT/SIGUSR1
	sigaction(SIGQUIT, &act, 0);
	sigaction(SIGUSR1, &act, 0);

	sigemptyset(&new);
	sigaddset(&new, SIGINT);
	sigemptyset(&wait);
	sigaddset(&wait, SIGUSR1);
	sigprocmask(SIG_BLOCK, &new, &old);

	//�ٽ�������ִ��  

	if(sigsuspend(&wait) != -1)//�����ڴ˴�������wait�źż��滻new�źż�������
															//����SIGUSR1�źţ�������������������𣻹��������źţ�
															//����SIGINT����ỽ�ѳ���ִ��sigsuspend��ԭ�Ӳ�����
															//ע�⣺�����sigaddset(&wait, SIGUSR1);�����û�У�
															//��˴����������κ��źţ��������κ��źž��ỽ�ѳ���
		printf("sigsuspend error\n");
	printf("After sigsuspend\n");
	sigprocmask(SIG_SETMASK, &old, NULL);
	return 0;
}
