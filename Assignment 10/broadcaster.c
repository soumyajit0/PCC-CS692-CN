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
  if (setsockopt(sid,SOL_SOCKET,SO_BROADCAST,&brd,sizeof(brd))!=0)
  {
    printf("Cannot set socket options...\n");
    close(sid);
    exit(1);
  }
  saddr.sin_family=AF_INET;
  saddr.sin_addr.s_addr=htonl(INADDR_BROADCAST);
  saddr.sin_port=htons(1234);
  printf("| Broadcaster Live |\n");
  while(1)
  {
    char message[MAX];
    printf("Enter message : ");
    scanf("%[^\n]%*c",message);
    sendto(sid,(void *)&message,strlen(message)+1,0,(struct sockaddr *)&saddr,sizeof(saddr));
    if (strcmp(message,"QUIT")==0)
    {
      printf("Broadcast terminated...\n");
      close(sid);
      exit(1);
    }
  }
  close(sid);
  return 0;
}
