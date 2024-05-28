#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#define MAX 100

int main(int argc,char **argv)
{
  char sip_addr[MAX];
  int port;
  if(argc!=3)
  {
    printf("Please provide the IP and Port no.s...\n");
    exit(1);
  }
  strcpy(sip_addr,argv[1]);
  port=atoi(argv[2]);
  int sid=socket(AF_INET,SOCK_DGRAM,0);
  if (sid<0)
  {
    printf("Cannot create Socket...\n");
    exit(1);
  }
  struct sockaddr_in saddr;
  saddr.sin_family=AF_INET;
  saddr.sin_addr.s_addr=inet_addr(sip_addr);
  saddr.sin_port=htons(port);
  printf("Client is Online...\n");
  while(1)
  {
    char dataword[MAX],codeword[MAX];
    printf("\nEnter the Dataword : ");
    scanf("%[^\n]%*c",dataword);
    sendto(sid,(void *)&dataword,strlen(dataword)+1,0,(struct sockaddr*)&saddr,sizeof(saddr));
    if(strcmp(dataword,"end")==0)
    {
      printf("Client terminated...\n");
      break;
    }
    int len=sizeof(saddr);
    recvfrom(sid,(void *)&codeword,MAX,0,(struct sockaddr*)&saddr,&len);
    printf("Codeword : %s\n",codeword);
  }
  close(sid);
  return 0;
}
