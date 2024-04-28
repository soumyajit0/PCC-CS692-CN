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
  int sid=socket(AF_INET,SOCK_STREAM,0);
  saddr.sin_family=AF_INET;
  saddr.sin_addr.s_addr=inet_addr(sip_addr);
  saddr.sin_port=htons(port);
  int res=connect(sid,(struct sockaddr *)&saddr,sizeof(saddr));
  if (res!=0)
  {
    printf("Cannot connect to Server\n");
    close(sid);
    exit(1);
  }
  char send[MAX],receive[MAX];
  printf("Client running...\n");
  printf("Enter a message for Process ID %d : ",getpid());
  scanf("%[^\n]%*c",send);
  write(sid,(void *)&send,strlen(send)+1);
  if (strcmp(send,"end")==0)
  {
    printf("Client terminated...\n");
    close(sid);
    exit(0);
  }
  printf("Message \"%s\" sent to the Server\n",send);
  read(sid,(void *)&receive,MAX);
  printf("Server response : %s\n\n",receive);
  close(sid);
  return 0;
}
