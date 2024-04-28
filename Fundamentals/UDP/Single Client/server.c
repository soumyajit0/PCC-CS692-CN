#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>

#define MAX 100

int main(int argc,char **argv)
{
  char sip_addr[MAX];
  int port;
  if (argc==1)
  {
    strcpy(sip_addr,"127.0.0.1");
    port=8080;
  }
  else
  {
    strcpy(sip_addr,argv[1]);
    port=atoi(argv[2]);
  }
  struct sockaddr_in saddr,caddr;
  int sid=socket(AF_INET,SOCK_DGRAM,0);
  saddr.sin_family=AF_INET;
  saddr.sin_addr.s_addr=inet_addr(sip_addr);
  saddr.sin_port=htons(port);
  int res=bind(sid,(struct sockaddr *)&saddr,sizeof(saddr));
  if (res!=0)
  {
    printf("Cannot bind to the Server\n");
    exit(1);
  }
  int size=sizeof(caddr);
  char send[MAX],receive[MAX];
  printf("Server running...\n");
  while(1)
  {
    recvfrom(sid,(void *)&receive,MAX,0,(struct sockaddr *)&caddr,&size);
    if (strcmp(receive,"end")==0)
    {
      printf("Server terminated...\n");
      break;
    }
    printf("Message \"%s\" received from the Client\n",receive);
    strcpy(send,"MESSAGE RECEIVED");
    sendto(sid,(void *)&send,strlen(send)+1,0,(struct sockaddr *)&caddr,sizeof(caddr));
  }
  close(sid);
  return 0;
}
