#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<time.h>
#include<pthread.h>

#define MAX 100

struct frame_struct
{
  unsigned int seq_num;
  char frame_body[MAX];
};

void *rcv(void *args)
{
  int cid=*(int *)args;
  int seq_rcv,ack_no=1;
  struct frame_struct frame;
  while(1)
  {
    recv(cid,(void *)&frame,sizeof(frame),0);
    seq_rcv=frame.seq_num;
    if (seq_rcv!=ack_no-1)
    {
      printf("\nReceived DUPLICATE frame:%d\n",seq_rcv);
      send(cid,(void *)&ack_no,sizeof(ack_no),0);
      printf("\nAcknowledgement %d sent\n",ack_no);
    }
    else
    {
      printf("\nReceived Sequence No.:%d\n",seq_rcv);
      int delay=rand()%5+1;
      sleep(delay);
      send(cid,(void *)&ack_no,sizeof(ack_no),0);
      printf("\nAcknowledgement %d sent\n",ack_no++);
    }
  }
  pthread_exit(NULL);
}

int main(int argc,char **argv)
{
  if(argc<1)
  {
    printf("Please provide a Port no.\n");
    exit(1);
  }
  int port=atoi(argv[1]);
  int sid=socket(AF_INET,SOCK_STREAM,0);
  if(sid<0)
  {
    printf("Error while creating Socket...\n");
    exit(1);
  }
  struct sockaddr_in saddr,caddr;
  int reuse=1;
  if(setsockopt(sid,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse))<0)
  {
    printf("Cannot set Socket options...\n");
    close(sid);
    exit(1);
  }
  saddr.sin_family=AF_INET;
  saddr.sin_addr.s_addr=INADDR_ANY;
  saddr.sin_port=htons(port);
 if(bind(sid,(struct sockaddr*)&saddr,sizeof(saddr))<0)
 {
  printf("Cannot bind to server...\n");
  close(sid);
  exit(1);
 }
 printf("bind: Success\n");
 listen(sid,5);
 printf("\nListening\n");
 int len=sizeof(caddr);
 int cid=accept(sid,(struct sockaddr*)&caddr,&len);
 printf("\nAccepted\n");
 pthread_t t;
 pthread_create(&t,NULL,&rcv,(void *)&cid);
 pthread_join(t,NULL);
 close(sid);
 return 0;
}
