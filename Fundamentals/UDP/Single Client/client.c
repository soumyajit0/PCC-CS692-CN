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
  struct sockaddr_in saddr;
  int sid=socket(AF_INET,SOCK_DGRAM,0);
  saddr.sin_family=AF_INET;
  saddr.sin_addr.s_addr=inet_addr(sip_addr);
  saddr.sin_port=htons(port);
  int size=sizeof(saddr);
  char send[MAX],receive[MAX];
  printf("Client running...\n");
  while(1)
  {
    printf("Enter a message : ");
    scanf("%[^\n]%*c",send);
    sendto(sid,(void *)&send,strlen(send)+1,0,(struct sockaddr *)&saddr,sizeof(saddr));
    if (strcmp(send,"end")==0)
    {
      printf("Client terminated...\n");
      break;
    }
    printf("Message \"%s\" sent to the Server\n",send);
    recvfrom(sid,(void *)&receive,MAX,0,(struct sockaddr *)&saddr,&size);
    printf("Server Response : %s\n\n",receive);
  }
  close(sid);
  return 0;
}
