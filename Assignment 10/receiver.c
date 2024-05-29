#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#define MAX 100

int main()
{
  struct sockaddr_in saddr,caddr;
  int sid=socket(AF_INET,SOCK_DGRAM,0);
  int reuse=1;
  if (setsockopt(sid,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse))!=0)
  {
    printf("Cannot set socket options...\n");
    close(sid);
    exit(1);
  }
  saddr.sin_family=AF_INET;
  saddr.sin_addr.s_addr=htonl(INADDR_ANY);
  saddr.sin_port=htons(1234);
  if(bind(sid,(struct sockaddr *)&saddr,sizeof(saddr))!=0)
  {
    printf("Cannot bind to server...\n");
    close(sid);
    exit(1);
  }
  printf("| Receiver Live |\n");
  int len=sizeof(caddr);
  while(1)
  {
    char message[MAX];
    recvfrom(sid,(void *)&message,MAX,0,(struct sockaddr *)&caddr,&len);
    if (strcmp(message,"QUIT")==0)
    {
      printf("Receiver terminated...\n");
      close(sid);
      exit(0);
    }
    printf("Message received : \"%s\"\n",message);
  }
  close(sid);
  return 0;
}
