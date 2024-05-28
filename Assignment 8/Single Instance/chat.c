#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<pthread.h>

#define MAX 100

int sid;
struct sockaddr_in xaddr,yaddr;

void *snd(void *args)
{
  while(1)
  {
    char mssg[MAX];
    scanf("%[^\n]%*c",mssg);
    if (strcmp(mssg,"end")==0)
    {
      printf("| Chat Offline |\n");
      close(sid);
      exit(0);
      pthread_exit(NULL);
    }
    sendto(sid,(void *)&mssg,strlen(mssg)+1,0,(struct sockaddr*)&yaddr,sizeof(yaddr));
  }
  pthread_exit(NULL);
}

void *rcv(void *args)
{
  while(1)
  {
    char mssg[MAX];
    int len=sizeof(yaddr);
    recvfrom(sid,(void *)&mssg,MAX,0,(struct sockaddr*)&yaddr,&len);
    printf("Message Received : %s\n",mssg);
  }
  pthread_exit(NULL);
}

int main(int argc,char **argv)
{
  char sip_addr1[MAX],sip_addr2[MAX];
  int port1,port2;
  if (argc!=5){
    printf("Please provide the IP and Port of Sender and Receiver...\n");
    exit(1);
  }
  strcpy(sip_addr1,argv[1]);
  port1=atoi(argv[2]);
  strcpy(sip_addr2,argv[3]);
  port2=atoi(argv[4]);
  sid=socket(AF_INET,SOCK_DGRAM,0);
  xaddr.sin_family=AF_INET;
  xaddr.sin_addr.s_addr=inet_addr(sip_addr1);
  xaddr.sin_port=htons(port1);
  yaddr.sin_family=AF_INET;
  yaddr.sin_addr.s_addr=inet_addr(sip_addr2);
  yaddr.sin_port=htons(port2);
  if(bind(sid,(struct sockaddr*)&xaddr,sizeof(xaddr))<0)
  {
    printf("Cannot bind to the Server...\n");
    close(sid);
  }
  printf("| Chat Online |\n");
  pthread_t t1,t2;
  pthread_create(&t1,NULL,&snd,NULL);
  pthread_create(&t2,NULL,&rcv,NULL);
  pthread_join(t1,NULL);
  pthread_join(t2,NULL);
  close(sid);
  return 0;
}
