#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>

#define MAX 100

void main(int argc,char ** argv){
	if(argc<3){
		printf("provide IP and port no!");
		exit(1);
	}
	
	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argv[2]));
	server.sin_addr.s_addr=inet_addr(argv[1]);
	
	int sockfd=socket(AF_INET,SOCK_DGRAM,0);
		
	socklen_t len=sizeof(server);
	char buff[512],result[512];
	while(1){
		bzero(buff,sizeof(buff));
		printf("Enter data: ");
		scanf(" %256[01][^\n]s",buff);
		sendto(sockfd,buff,sizeof(buff),0,(struct sockaddr*)&server,sizeof(server));

		recvfrom(sockfd,result,sizeof(result),0,(struct sockaddr*)&server,&len);
		printf("Code word received: %s\n",result);
	}
}
