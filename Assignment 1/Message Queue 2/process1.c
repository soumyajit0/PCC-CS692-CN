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

int main(){
	key_t key=ftok("msg_key",2832);
	int msgid=msgget(key,0666|IPC_CREAT);
	if (msgid==-1){
		printf("Cannot connect to Server...\n");
		exit(1);
	}
	int i=0;
	while(1){
		struct message_struct nameinput,rollinput;
		nameinput.message_type=1;
		rollinput.message_type=2;
		
		printf("Enter the name of student(%d) : ",i+1);
		char name[MAX];
		scanf("%s",name);
		getchar();
		strcpy(nameinput.message_body,name);
		msgsnd(msgid,(void*)&nameinput,MAX,0);
		
		printf("Enter the roll of student(%d) : ",i+1);
		char roll[MAX];
		scanf("%s",roll);
		getchar();
		strcpy(rollinput.message_body,roll);
		msgsnd(msgid,(void*)&rollinput,MAX,0);
		
		if (strcmp(name,"end")==0 && strcmp(roll,"end")==0){
			printf("\nTerminated...\n\n");
			break;
		}
		
		printf("Message sent to Server...\n\n");
	}
	msgctl(msgid,IPC_RMID,0);
	
	msgid=msgget((key_t)28321,0666|IPC_CREAT);
	if (msgid==-1){
		printf("Cannot connect to Server...\n");
		exit(1);
	}
	i=0;
	char sortedname[MAX][MAX],sortedroll[MAX][MAX];
	while(1){
		struct message_struct nameoutput;
		msgrcv(msgid,(void*)&nameoutput,MAX,1,0);
		if (strcmp(nameoutput.message_body,"end")==0){
			break;
		}
		strcpy(sortedname[i++],nameoutput.message_body);
	}
	msgctl(msgid,IPC_RMID,0);
	
	msgid=msgget((key_t)28322,0666|IPC_CREAT);
	if (msgid==-1){
		printf("Cannot connect to Server...\n");
		exit(1);
	}
	i=0;
	while(1){
		struct message_struct rolloutput;
		msgrcv(msgid,(void*)&rolloutput,MAX,1,0);
		if (strcmp(rolloutput.message_body,"end")==0){
			break;
		}
		strcpy(sortedroll[i++],rolloutput.message_body);
	}
	msgctl(msgid,IPC_RMID,0);
	
	printf("Sorted Name\tSorted Roll No.\n");
	int j;
	for(j=0;j<i;j++)
		printf("%s\t%s\n",sortedname[j],sortedroll[j]);
	printf("\n");
	
	return 0;
}
