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

void sort(char name[MAX][MAX],int n){
	int i,j;
	for(i=0;i<n-1;i++){
		for(j=i+1;j<n;j++){
			if (strcmp(name[j],name[i])<0){
				char temp[MAX];
				strcpy(temp,name[i]);
				strcpy(name[i],name[j]);
				strcpy(name[j],temp);
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
	
	char name[MAX][MAX];
	int i=0;
	printf("Waiting for Client...\n\n");
	while(1){
		struct message_struct nameinput;
		msgrcv(msgid,(void*)&nameinput,MAX,1,0);
		if (strcmp(nameinput.message_body,"end")==0)
			break;
		strcpy(name[i++],nameinput.message_body);
		printf("Received name : %s\n",nameinput.message_body);
	}
	
	sort(name,i);
	
	msgid=msgget((key_t)28321,0666|IPC_CREAT);
	if (msgid==-1){
		printf("Cannot connect to Server...\n");
		exit(1);
	}
	int j;
	for(j=0;j<i;j++){
		struct message_struct nameoutput;
		nameoutput.message_type=1;
		strcpy(nameoutput.message_body,name[j]);
		msgsnd(msgid,(void*)&nameoutput,MAX,0);
	}
	
	struct message_struct nameoutput;
	nameoutput.message_type=1;
	strcpy(nameoutput.message_body,"end");
	msgsnd(msgid,(void*)&nameoutput,MAX,0);
	
	return 0;
}
