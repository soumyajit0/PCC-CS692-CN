#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<pthread.h>

#define MAX 100

int lock=0;

struct sock_struct
{
  int id;
  struct sockaddr_in addr;
};

void *snd(void *arg)
{
  char message[MAX];
  struct sock_struct sock=*((struct sock_struct *)arg);
  int sid=sock.id;
  struct sockaddr_in caddr=sock.addr;
  while(1)
  {
    scanf("%[^\n]%*c",message);
    lock=1;
    if (strcmp(message,"end")==0)
    {
      printf("| Chat Offline |");
      close(sid);
      exit(0);
      pthread_exit(NULL);
    }
    sendto(sid,(void *)&message,strlen(message)+1,0,(struct sockaddr *)&caddr,sizeof(caddr));
  }
  pthread_exit(NULL);
}

void *receive(void *arg)
{
  char message[MAX];
  struct sock_struct sock=*((struct sock_struct *)arg);
  int sid=sock.id;
  struct sockaddr_in caddr=sock.addr;
  while(1)
  {
    int size=sizeof(caddr);
    recvfrom(sid,(void *)&message,MAX,0,(struct sockaddr *)&caddr,&size);
    if (lock==0)
      printf("Message received \"%s\"\n",message);
    lock=0;
  }
  pthread_exit(NULL);
}

int main(int argc,char **argv)
{
  char sip_addr[MAX];
  int port;
  if (argc==1)
  {
    strcpy(sip_addr,"224.0.0.1");
    port=1234;
  }
  else
  {
    strcpy(sip_addr,argv[1]);
    port=atoi(argv[2]);
  }
  int sid=socket(AF_INET,SOCK_DGRAM,0);
  struct sockaddr_in multi;
  multi.sin_family=AF_INET;
  multi.sin_addr.s_addr=inet_addr(sip_addr);
  multi.sin_port=htons(port);
  int reuse=1;
  int res=setsockopt(sid,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
  if (res!=0)
  {
    printf("Error setting up Socket\n");
    close(sid);
    exit(1);
  }
  res=bind(sid,(struct sockaddr *)&multi,sizeof(multi));
  if (res!=0)
  {
    printf("Error binding Socket\n");
    close(sid);
    exit(1);
  }
  struct ip_mreq mreq;
  mreq.imr_multiaddr.s_addr=inet_addr(sip_addr);
  mreq.imr_interface.s_addr=INADDR_ANY;
  res=setsockopt(sid,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));
  if (res!=0)
  {
    printf("Error assigning interface\n");
    close(sid);
    exit(1);
  }
  pthread_t t1,t2;
  printf("| Chat Online |\n");
  struct sock_struct sock;
  sock.id=sid;
  sock.addr=multi;
  pthread_create(&t1,NULL,snd,(void *)&sock);
  pthread_create(&t2,NULL,receive,(void *)&sock);
  pthread_join(t1,NULL);
  pthread_join(t2,NULL);
  return 0;
}
