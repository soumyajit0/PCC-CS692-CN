#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<math.h>

#define MAX 100

void hamming(char *dataword,char *codeword)
{
  int m=strlen(dataword),power=0;
  while(pow(2,power)<power+m+1)
    power++;
  int sz=m+power;
  int i,j=0;
  for(i=0;i<sz;i++)
  {
    if (ceil(log2(sz-i))==floor(log2(sz-i)))
      codeword[i]='p';
    else
      codeword[i]=dataword[j++];
  }
  codeword[i]='\0';
  int bitsCal=0;
  while(bitsCal<power)
  {
    int pos=pow(2,bitsCal);
    int bitsPos=pos;
    int ones=0;
    while(bitsPos<=sz)
    {
      j=0;
      while(j<pos)
      {
        if(codeword[sz-(bitsPos+j)]=='1')
          ones++;
        j++;
      }
      bitsPos+=2*pos;
    }
    if (ones%2==0)
      codeword[sz-pos]='0';
    else
      codeword[sz-pos]='1';
    bitsCal++;
  }
}

int main(int argc,char **argv)
{
  char sip_addr[MAX];
  int port;
  if(argc!=3)
  {
    printf("Please provide the IP and Port no.s...\n");
    exit(1);
  }
  strcpy(sip_addr,argv[1]);
  port=atoi(argv[2]);
  int sid=socket(AF_INET,SOCK_DGRAM,0);
  if (sid<0)
  {
    printf("Cannot create Socket...\n");
    exit(1);
  }
  struct sockaddr_in saddr,caddr;
  saddr.sin_family=AF_INET;
  saddr.sin_addr.s_addr=inet_addr(sip_addr);
  saddr.sin_port=htons(port);
  if (bind(sid,(struct sockaddr*)&saddr,sizeof(saddr))<0)
  {
    printf("Cannot bind to the server...\n");
    close(sid);
    exit(1);
  }
  printf("Server is Online...\n");
  while(1)
  {
    char dataword[MAX],codeword[MAX];
    int len=sizeof(caddr);
    recvfrom(sid,(void *)&dataword,MAX,0,(struct sockaddr*)&caddr,&len);
    if(strcmp(dataword,"end")==0)
    {
      printf("Server terminated...\n");
      break;
    }
    printf("\nDataword : %s\n",dataword);
    //strcpy(codeword,dataword);
    hamming(dataword,codeword);
    printf("Codeword : %s\n",codeword);
    sendto(sid,(void *)&codeword,strlen(codeword)+1,0,(struct sockaddr*)&caddr,sizeof(caddr));
  }
  close(sid);
  return 0;
}
