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
  struct sockaddr_in saddr;
  int sid=socket(AF_INET,SOCK_DGRAM,0);
  int brd=1;
  if (setsockopt(sid,SOL_SOCKET,SO_REUSEADDR,&brd,sizeof(brd))!=0)
  {
    printf("Cannot set socket options...\n");
    close(sid);
    exit(1);
  }
  saddr.sin_family=AF_INET;
  saddr.sin_addr.s_addr=htonl(INADDR_BROADCAST);
  saddr.sin_port=htons(1234);
  bind(sid,(struct sockaddr *)&saddr,sizeof(saddr));
  printf("| Receiver Live |\n");
  while(1)
  {
    char message[MAX];
    int len=sizeof(saddr);
    recvfrom(sid,(void *)&message,MAX,0,(struct sockaddr *)&saddr,&len);
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
