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
  printf("Receiver Process ->\n");
  while(1){
    msgrcv(msgid,(void *)&message_send,MAX,1,0);
    if (strcmp(message_send.message_body,"end")==0)
    {
      printf("Receiver Process terminated...\n");
      break;
    }
    printf("Message \"%s\" received from the Sender Process\n",message_send.message_body);
  }
  msgctl(msgid,IPC_RMID,0);
  return 0;
}
