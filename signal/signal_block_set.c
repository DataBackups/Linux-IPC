#include <stdio.h>
#include <signal.h>
#include <unistd.h>

//int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
//
//
//how, as follows.
//
//SIG_BLOCK
//	The set of blocked signals is the union of the current set and the set argument.
//
//SIG_UNBLOCK
//   The signals in set are removed from the current set of blocked signals.  It is permissible to attempt to unblock  a  signal which is not blocked.
//
//SIG_SETMASK
//	The set of blocked signals is set to the argument set.
//
//If oldset is non-null, the previous value of the signal mask is stored in oldset.
//If set is NULL, then the signal mask is unchanged (i.e., how is ignored), but the current value of the signal mask is nevertheless returned in oldset (if it is not NULL).


void action(int signum, siginfo_t *info, void *myact)
{
	printf("receive signal %d\n", signum);	    
}

int main()
{
	sigset_t new_mask, old_mask, pending_mask;
	struct sigaction act;
	int ret;
	int signum = 40;

	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = action;

	ret = sigaction(40, &act, NULL);	
	if (ret < 0)
		printf("install signal 40 error\n");

	sigemptyset(&new_mask);
	sigaddset(&new_mask, 40);//���ź�40��ӵ��ź�������

	ret = sigprocmask(SIG_BLOCK, &new_mask, &old_mask);//��new_mask�е��ź�40�������������浱ǰ���ź������ֵ�old_mask��
	if (ret < 0)
		printf("sigprocmask error\n");
	else 
		printf("block signal 40\n");

	sleep(10);//���ڼ��յ��ź�40�����̲����ж�������Ϊ���������ˣ� �ָ����κ������ִ��

	printf("now begin to get pending mask and unblock 40\n");

	ret = sigpending(&pending_mask);//����ź�������δ����
	if (ret < 0)
		printf("sigpending error\n");
	if (sigismember(&pending_mask, 40) < 0) {//����ź�40�Ƿ���penging״̬
		printf("40 pending error");
	} else {
		printf("pending signal 40\n");	
	}

	sigprocmask(SIG_SETMASK, &old_mask, NULL);//�ָ������ε��ź�

	printf("40 unblocked\n");

	sleep(10);

	return 0;
}
/*
*slzsource/ddu/LinuxIPC/signal# ./a.out   һ���ն����У�����һ���ն�ִ��kill -s 40 pid 
*block signal 40
*now begin to get pending mask and unblock 40
*pending signal 40
*receive signal 40
*40 unblocked
*/
