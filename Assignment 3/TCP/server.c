#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<math.h>

#define MAX 100
#define int long long int

struct message_struct
{
  char dataword[MAX];
  char divisor[MAX];
  char codeword[MAX];
  char remainder[MAX];
};

void reverse(char *str)
{
  int i=0,j=strlen(str)-1;
  while(i<j)
  {
    char temp=str[i];
    str[i]=str[j];
    str[j]=temp;
    i++;
    j--;
  }
}

int toDec(char *str)
{
  int i,power=strlen(str)-1,n=0;
  for(i=0;i<strlen(str);i++)
  {
    n+=pow(2,power)*(str[i]-'0');
    power--;
  }
  return n;
}

void toBin(int dec,char *bin)
{
  if (dec==0)
  {
    strcpy(bin,"0");
    return;
  }
  int i=0;
  while(dec>0)
  {
    bin[i++]=(char)(dec%2+'0');
    dec/=2;
  }
  bin[i]='\0';
  reverse(bin);
}

void CRC(char *datawordBin,char *divisorBin,char *codewordBin,char *remainderBin)
{
  int divisorLen=strlen(divisorBin);
  int datawordDec=toDec(datawordBin);
  int divisorDec=toDec(divisorBin);
  int dividendDec=datawordDec << (divisorLen-1);
  int shft=(int)(ceil(log2(dividendDec+1))-divisorLen);
  while(dividendDec>=divisorDec || shft>=0)
  {
    int remDec=(dividendDec>>shft)^divisorDec;
    dividendDec=(dividendDec&((1ll<<shft)-1))|(remDec<<shft);
    shft=(int)(ceil(log2(dividendDec+1))-divisorLen);
  }
  int codewordDec=(datawordDec<<(divisorLen-1))|dividendDec;
  toBin(codewordDec,codewordBin);
  strcpy(remainderBin,codewordBin+strlen(datawordBin));
}

int main(int argc,char **argv)
{
  char sip_addr[MAX];
  int port;
  if(argc!=3)
  {
    printf("Please provide IP and Port No....\n");
    exit(1);
  }
  strcpy(sip_addr,argv[1]);
  port=atoi(argv[2]);
  int sid=socket(AF_INET,SOCK_STREAM,0);
  if (sid<0)
  {
    printf("Cannot create socket...\n");
    exit(1);
  }
  struct sockaddr_in saddr,caddr;
  saddr.sin_family=AF_INET;
  saddr.sin_addr.s_addr=inet_addr(sip_addr);
  saddr.sin_port=htons(port);
  if (bind(sid,(struct sockaddr *)&saddr,sizeof(saddr))<0)
  {
    printf("Cannot bind to the server...\n");
    close(sid);
    exit(1);
  }
  listen(sid,5);
  int32_t len=sizeof(caddr);
  int cid=accept(sid,(struct sockaddr *)&caddr,&len);
  printf("| Server Online |\n");
  while(1)
  {
    struct message_struct message;
    read(cid,(void *)&message,sizeof(message));
    if (strcmp(message.dataword,"end")==0)
    {
      printf("| Server Offline |\n");
      close(cid);
      break;
    }
    printf("\nDataword : %s\n",message.dataword);
    printf("Divisor : %s\n",message.divisor);
    CRC(message.dataword,message.divisor,message.codeword,message.remainder);
    printf("Codeword : %s\n",message.codeword);
    printf("Remainder : %s\n",message.remainder);
    write(cid,(void *)&message,sizeof(message));
  }
  close(sid);
  return 0;
}
