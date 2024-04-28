#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/un.h>
#include<sys/stat.h>

#define MAX 100

int main()
{
  struct sockaddr_un saddr,caddr;
  int sid=socket(AF_UNIX,SOCK_STREAM,0);
  saddr.sun_family=AF_UNIX;
  strcpy(saddr.sun_path,"socket_server");
  int res=bind(sid,(struct sockaddr *)&saddr,sizeof(saddr));
  if (res!=0)
  {
    printf("Cannot bind Socket to Server\n");
    close(sid);
    exit(1);
  }
  listen(sid,5);
  int size=sizeof(caddr);
  int cid=accept(sid,(struct sockaddr *)&caddr,&size);
  char send[MAX],receive[MAX];
  printf("Server running ->\n");
  while(1)
  {
    read(cid,(void *)&receive,MAX);
    if (strcmp(receive,"end")==0)
    {
      printf("Server terminated...\n");
      break;
    }
    printf("Server received \"%s\" from the Client\n",receive);
    strcpy(send,"MESSAGE RECEIVED");
    write(cid,(void *)&send,strlen(send)+1);
  }
  close(cid);
  close(sid);
  return 0;
}
