#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>

#define MAX 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

void strrev(char *s){
	int l=strlen(s)-1;
	int i=0;
	while(i<l){
		char t=s[i];
		s[i]=s[l];
		s[l]=t;
		i++;
		l--;
	}
}

void hamming(char *data,char* result){
	//The data received is assumed to be in 1 based reverse indexing i.e.
	// data-->  10101011
	// index--> 87654321
	bzero(result,sizeof(result));
	char temp[512]={'\0'};
	int end;
	int r=1,m=strlen(data);
	end=m-1;
	while((1<<r)<m+r+1)		//This finds number of bits required for this msg len
		r++;
	int msglen=m+r,i,j;
	temp[0]='0';

	for(i=1;i<=msglen;i++){		//we need 1 based indexing and also reverse the data received
		if((i&(i-1))==0)	// this operation is a easy way to check if a value if power of 2. only power of 2 gives (val bitwise and val-1)==0
			temp[i]='0';
		else
			temp[i]=data[end--];
	}

	for(i=1;i<=msglen;i++){
		if((i&(i-1))==0){	// check for power of 2
			int onecnt=0;
			for(j=i+1;j<=msglen;j++){
				if(j&i)
					onecnt+= temp[j]=='1'? 1:0;
				
			}
			temp[i]= (onecnt&1)? '1':'0';
		}
		
	}
	
	// The temp has our required result, but in 1 based indexing
	char *final=calloc(msglen+1,1);
	for(int i=1;i<=msglen;i++)	// copy the result to a 0 based indexing
		final[i-1]=temp[i];
		
	strrev(final);	// reverse the result back to a reverse indexing
	strcpy(result,final);
}

void main(int argc,char ** argv){
	if(argc<2){
		printf("provide port no!");
		exit(1);
	}
	
	struct sockaddr_in server,client;
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argv[1]));
	server.sin_addr.s_addr=INADDR_ANY;
	
	int sockfd=socket(AF_INET,SOCK_DGRAM,0);
	
	bind(sockfd,(struct sockaddr*)&server,sizeof(server));
	
	socklen_t len=sizeof(client);
	char buff[512],result[512];
	while(1){
		bzero(buff,sizeof(buff));
		recvfrom(sockfd,buff,sizeof(buff),0,(struct sockaddr*)&client,&len);
		printf("Data received: %s\n",buff);
		hamming(buff,result);
		sendto(sockfd,result,sizeof(result),0,(struct sockaddr*)&client,len);
		printf("Code word sent: %s\n",result);
	}
}
