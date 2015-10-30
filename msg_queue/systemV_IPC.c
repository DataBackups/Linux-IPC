#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

#define MSG_W 2
#define BUF_SIZE 512

typedef struct msgbuf {
	long mtype;
	char mdata[BUF_SIZE];
} mymsg_t;


void Usage(char *input)
{
	fprintf(stdout, "Usage: %s [-e] [-t type]\n", input); 
}

int main(int argc, char **argv)
{
	key_t key;
	int c, flags;
	int mqid;
	size_t msglen;
	long msgtype;
	mymsg_t *ptr;
	mymsg_t *buf = NULL;
	int ret;
	char *pathname = "/media/mdd0/slzsource/ddu/LinuxIPC/msg/tmp";

	// key_t ftok(const char *pathname, int proj_id); proj_id 0 ~ 255
	key = ftok(pathname, MSG_W);
	if (key < 0)
		fprintf(stderr, "ftok error %s\n", strerror(errno));

	flags = IPC_CREAT;
	while ((c = getopt(argc, argv, "eht:")) != -1) {
		switch (c) {
		case 'e':
			flags |= IPC_EXCL;
			break;
		case 't':
			msgtype = atoi(optarg);
			fprintf(stdout, "-t %ld\n", msgtype);
			break;
		case 'h':
			Usage(argv[0]);
			break;
		case '?':
			fprintf(stdout, "Invalid '%c'\n", optopt);
			Usage(argv[0]);
			break;
		default:
			Usage(argv[0]);
			break;
		}
	}

	//int msgget(key_t key, int msgflg);
	mqid = msgget(key, flags);
	if (mqid < 0)
		fprintf(stderr, "msgget error %s\n", strerror(errno));
	else 
		fprintf(stdout, "msgget ok\n");
	
	//create a message
	ptr = malloc(sizeof(long) + BUF_SIZE);
	ptr->mtype = msgtype;
	fprintf(stdout, "msgtype:%ld\n", ptr->mtype);
	snprintf(ptr->mdata, BUF_SIZE, "hi boy~");

	msglen = strlen(ptr->mdata);
	//int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
	ret = msgsnd(mqid, ptr, msglen, IPC_NOWAIT);
	if (ret < 0)
		fprintf(stderr, "msgsnd fail %s\n", strerror(errno));
	else
		fprintf(stdout, "msgsnd ok\n"); 

	buf = malloc(BUF_SIZE + sizeof(long));
	ret = msgrcv(mqid, buf, BUF_SIZE + sizeof(long), msgtype, IPC_NOWAIT);
	if (ret < 0)
		fprintf(stderr, "msgrcv error %s\n", strerror(errno));
	else
		fprintf(stdout, "receive: %d bytes, type: %ld, data: %s\n", ret, buf->mtype, buf->mdata);

	ret = msgctl(mqid, IPC_RMID, NULL);
	if (ret < 0)
		fprintf(stderr, "msgctl error %s\n", strerror(errno));
	else
		fprintf(stdout, "msgctl ok\n");

	return 0;	
}
