#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/dispatch.h>
#include <errno.h>
#include <string.h>
#include <sys/neutrino.h>

int set_priority(int priority){
    int policy;
    struct sched_param param;

    if(priority < 1 || priority > 63){
        return -1;
    }

    pthread_getschedparam(pthread_self(), &policy, &param);
    param.sched_priority = priority;
    return pthread_setschedparam(pthread_self(), policy, &param);
}

int get_priority(){
    int policy;
    struct sched_param param;
    pthread_getschedparam(pthread_self(), &policy, &param);
    return param.sched_curpriority;
}

int main(int argc, char *argv[]) {
	set_priority(30);

	// Create channel
	int chID = ChannelCreate(_NTO_CHF_FIXED_PRIORITY);
	pid_t prID = getpid();
	//printf("%d", prID);
	FILE *fp;
	fp = fopen("/root/serverinfo.txt", "w");

	fprintf(fp, "%d %d", chID, prID);
	fclose(fp);

	char recvMsg[256];
	char replyMsg[256];
	int msgID;

	struct _msg_info info;
	printf("Staring server \n");

	while(1){
		//printf("server spinning\n");

		msgID = MsgReceive(chID, &recvMsg, 256, &info);
		printf("got '%s' from coID: %d w/ pri: %d \n", (char*)recvMsg, info.coid, info.priority);
		int pri = get_priority();
		printf("current server pri: %d \n", pri);
		sprintf(replyMsg,"General Kenobi");
		MsgReply(msgID, EOK, &replyMsg, 256);

	}

	return EXIT_SUCCESS;
}
