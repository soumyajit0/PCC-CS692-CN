#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX 100

struct message_struct{
	long int message_type;
	char message_body[MAX];
};

void sort(char roll[MAX][MAX],int n){
	int i,j;
	for(i=0;i<n-1;i++){
		for(j=i+1;j<n;j++){
			if (strcmp(roll[j],roll[i])<0){
				char temp[MAX];
				strcpy(temp,roll[i]);
				strcpy(roll[i],roll[j]);
				strcpy(roll[j],temp);
			}
		}
	}
}

int main(){
	key_t key=ftok("msg_key",2832);
	int msgid=msgget(key,0666|IPC_CREAT);
	if (msgid==-1){
		printf("Cannot connect to Server...\n");
		exit(1);
	}
	
	char roll[MAX][MAX];
	int i=0;
	printf("Waiting for Client...\n\n");
	while(1){
		struct message_struct rollinput;
		msgrcv(msgid,(void*)&rollinput,MAX,2,0);
		if (strcmp(rollinput.message_body,"end")==0)
			break;
		strcpy(roll[i++],rollinput.message_body);
		printf("Received name : %s\n",rollinput.message_body);
	}
	
	sort(roll,i);
	
	msgid=msgget((key_t)28322,0666|IPC_CREAT);
	if (msgid==-1){
		printf("Cannot connect to Server...\n");
		exit(1);
	}
	int j;
	for(j=0;j<i;j++){
		struct message_struct rolloutput;
		rolloutput.message_type=1;
		strcpy(rolloutput.message_body,roll[j]);
		msgsnd(msgid,(void*)&rolloutput,MAX,0);
	}
	
	struct message_struct rolloutput;
	rolloutput.message_type=1;
	strcpy(rolloutput.message_body,"end");
	msgsnd(msgid,(void*)&rolloutput,MAX,0);
	
	return 0;
}
