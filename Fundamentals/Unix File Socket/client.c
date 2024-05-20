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
  struct sockaddr_un saddr;
  int sid=socket(AF_UNIX,SOCK_STREAM,0);
  saddr.sun_family=AF_UNIX;
  strcpy(saddr.sun_path,"socket_server");
  int res=connect(sid,(struct sockaddr *)&saddr,sizeof(saddr));
  if (res!=0)
  {
    printf("Cannot connect to the Server\n");
    close(sid);
    exit(1);
  }
  char send[MAX],receive[MAX];
  printf("Client running ->\n");
  while(1)
  {
    printf("Enter a message : ");
    scanf("%[^\n]%*c",send);
    write(sid,(void *)&send,strlen(send)+1);
    if (strcmp(send,"end")==0)
    {
      printf("Client terminated...\n");
      break;
    }
    printf("Message \"%s\" sent to the Server\n",send);
    read(sid,receive,MAX);
    printf("Server Response : %s\n\n",receive);
  }
  close(sid);
  return 0;
}
