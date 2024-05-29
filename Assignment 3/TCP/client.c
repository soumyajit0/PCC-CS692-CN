#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<fcntl.h>

#define MAX 100

struct message_struct
{
  char dataword[MAX];
  char divisor[MAX];
  char codeword[MAX];
  char remainder[MAX];
};

int main(int argc,char **argv)
{
  char sip_addr[MAX];
  int port;
  if(argc!=3)
  {
    printf("Please provide IP and Port No....\n");
    exit(1);
  }
  strcpy(sip_addr,argv[1]);
  port=atoi(argv[2]);
  int sid=socket(AF_INET,SOCK_STREAM,0);
  if (sid<0)
  {
    printf("Cannot create socket...\n");
    exit(1);
  }
  struct sockaddr_in saddr;
  saddr.sin_family=AF_INET;
  saddr.sin_addr.s_addr=inet_addr(sip_addr);
  saddr.sin_port=htons(port);
  if(connect(sid,(struct sockaddr*)&saddr,sizeof(saddr))<0)
  {
    printf("Cannot connect to server...\n");
    close(sid);
    exit(1);
  }
  printf("| Client Online |\n");
  while(1)
  {
    struct message_struct message;
    printf("\nEnter the Dataword : ");
    scanf("%[^\n]%*c",message.dataword);
    if (strcmp(message.dataword,"end")==0)
    {
      printf("| Client Offline |\n");
      write(sid,(void *)&message,sizeof(message));
      close(sid);
      exit(1);
    }
    printf("Enter the Divisor : ");
    scanf("%[^\n]%*c",message.divisor);
    write(sid,(void *)&message,sizeof(message));
    int len=sizeof(saddr);
    read(sid,(void *)&message,sizeof(message));
    printf("Codeword : %s\n",message.codeword);
    printf("Remainder : %s\n",message.remainder);
  }
}
