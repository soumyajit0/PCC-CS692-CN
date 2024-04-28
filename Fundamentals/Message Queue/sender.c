#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/msg.h>

#define MAX 100
#define KEY 2832

struct message_struct
{
  long int message_type;
  char message_body[MAX];
};

int main()
{
  int msgid=msgget((key_t)KEY,0666|IPC_CREAT);
  struct message_struct message_send;
  printf("Sender Process ->\n");
  char message[MAX];
  while(1)
  {
    printf("Enter the message to send : ");
    scanf("%[^\n]%*c",message);
    message_send.message_type=1;
    strcpy(message_send.message_body,message);
    msgsnd(msgid,(void *)&message_send,sizeof(message_send.message_body),0);
    if (strcmp(message,"end")==0)
    {
      printf("Sender Process terminated...\n");
      break;
    }
    printf("Message \"%s\" sent to the Receiver Process\n",message);
  }
  msgctl(msgid,IPC_RMID,0);
  return 0;
}
