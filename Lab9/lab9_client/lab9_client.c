#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/dispatch.h>
#include <pthread.h>

int chID;
pid_t prIDserver;

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


void* spawnClient(void* args){
	int* priP = (int*)args;


	char msg[256];
	char replyMsg[256];
	int conID = ConnectAttach(0, prIDserver, chID, 0, 0);
	printf("conID: %d\n", conID);

	set_priority(*priP);

	int i = 0;
	for(i = 0; i<5; i++){
		sprintf(msg, "Hello there");
		printf("Client sending: %s \n", msg);
		MsgSend(conID, &msg, 256, &replyMsg,256);
		printf("Server replied with: %s \n", (char*)replyMsg);
	}

	ConnectDetach(conID);
}

int main(int argc, char *argv[]) {

	set_priority(62);

	FILE *fp;
	fp = fopen("/root/serverinfo.txt", "r");
	//printf("file opened\n");
	rewind(fp);
	fscanf(fp, "%d %d", &chID, &prIDserver);
	//printf("file read\n");
	fclose(fp);
	//printf("file closed\n");

	pthread_t clients[4];
	int priorities[4] = {2,3,60,61};
	int i;
	for (i = 0; i<4; i++){
		pthread_create(&clients[i], NULL, spawnClient, &priorities[i]);
	}

	for (i = 4; i>0; i--){
		pthread_join(clients[i-1], NULL);
	}




	return EXIT_SUCCESS;
}
