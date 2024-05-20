#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<pthread.h>

#define MAX 100

int sid;
struct sockaddr_in caddr;

void *snd(void *args)
{
  while(1)
  {
    char message[MAX];
    scanf("%[^\n]%*c",message);
    if (strcmp(message,"end")==0)
    {
      close(sid);
      printf("| Chat Offline |\n");
      exit(0);
      pthread_exit(NULL);
    }
    sendto(sid,(void *)&message,strlen(message)+1,0,(struct sockaddr *)&caddr,sizeof(caddr));
  }
  pthread_exit(NULL);
}

void *receive(void *args)
{
  while(1)
  {
    char message[MAX];
    int size=sizeof(caddr);
    recvfrom(sid,(void *)&message,MAX,0,(struct sockaddr *)&caddr,&size);
    printf("Contact 1 : \"%s\"\n",message);
  }
  pthread_exit(NULL);
}

int main(int argc,char **argv)
{
  char sip_addr[MAX];
  if (argc==1)
  {
    strcpy(sip_addr,"127.0.0.1");
  }
  else
  {
    strcpy(sip_addr,argv[1]);
  }
  sid=socket(AF_INET,SOCK_DGRAM,0);
  struct sockaddr_in xaddr,yaddr;
  xaddr.sin_family=AF_INET;
  yaddr.sin_family=AF_INET;
  xaddr.sin_addr.s_addr=inet_addr(sip_addr);
  yaddr.sin_addr.s_addr=inet_addr(sip_addr);
  xaddr.sin_port=htons(1235);
  yaddr.sin_port=htons(1234);
  bind(sid,(struct sockaddr *)&yaddr,sizeof(yaddr));
  caddr=yaddr;
  printf("| Chat Online |\n");
  pthread_t t1,t2;
  pthread_create(&t1,NULL,snd,NULL);
  pthread_create(&t2,NULL,receive,NULL);
  pthread_join(t1,NULL);
  pthread_join(t2,NULL);
  close(sid);
  return 0;
}
