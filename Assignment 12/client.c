#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<time.h>

#define MAX 100
#define TIMEOUT 2

struct frame_struct
{
  unsigned int seq_num;
  char frame_body[MAX];
};

int main(int argc,char **argv)
{
  if(argc<2)
  {
    printf("Please provide the IP and Port...\n");
    exit(1);
  }
  char sip_addr[MAX];
  strcpy(sip_addr,argv[1]);
  int port=atoi(argv[2]);
  int sid=socket(AF_INET,SOCK_STREAM,0);
  if(sid<0)
  {
    printf("Error while creating the Socket...\n");
    exit(1);
  }
  struct timeval tv;
  tv.tv_sec=TIMEOUT;
  tv.tv_usec=0;
  if(setsockopt(sid,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv))<0)
  {
    printf("Error while setting Socket options...\n");
    close(sid);
    exit(1);
  }
  struct sockaddr_in saddr;
  saddr.sin_family=AF_INET;
  saddr.sin_addr.s_addr=inet_addr(sip_addr);
  saddr.sin_port=htons(port);
  if(connect(sid,(struct sockaddr*)&saddr,sizeof(saddr))<0)
  {
    printf("Error while connecting to the Server...\n");
    close(sid);
    exit(1);
  }
  printf("connect: Success\n");
  int seq_no=0,ack_rcv;
  int timer=1;
  int ack=1;
  char *data="Aopz jvkl pz jvwplk myvt Zvbtfhqpa";
  while(1)
  {
    struct frame_struct frame;
    frame.seq_num=seq_no;
    strcpy(frame.frame_body,data);
    send(sid,(void *)&frame,sizeof(frame),0);
    if(ack==1)
      printf("\nSending Sequence number: %d\n",seq_no);
    else
    {
      printf("\nRe-Sending Sequence number: %d\n",seq_no);
      ack=1;
    }
    printf("Started Timer %d\n",timer++);
    int bytesRead=recv(sid,(void *)&ack_rcv,sizeof(ack_rcv),0);
    if(bytesRead<=0||ack_rcv!=seq_no+1)
    {
      printf("Acknowledgement not Received. Timer timed out.\n");
      ack=0;
    }
    else
    {
      printf("Acknowledgment %d Received\n",ack_rcv);
      seq_no++;
      ack=1;
    }
  }
  close(sid);
  return 0;
}
