#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include<sys/types.h>
#include<sys/socket.h>

#include<unistd.h>

#include<netinet/in.h>
#include<arpa/inet.h>

#define MAX 100

int main(int ac, char **av){
	struct sockaddr_in saddr;
	char sip_addr[MAX],bit[MAX],ans[MAX];
	if (ac==1)
		strcpy(sip_addr,"127.0.0.1");
	else
		strcpy(sip_addr,av[1]);
	int sid=socket(AF_INET,SOCK_STREAM,0);
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr=inet_addr(sip_addr);
	saddr.sin_port=htons(1234);
	int res=connect(sid,(struct sockaddr *)&saddr,sizeof(saddr));
	if (res==-1){
		printf("Cannot connect to the server...\n");
		exit(1);
	}
	while(1){
		printf("Enter a Bit-Stream : ");
		scanf("%s",bit);
		getchar();
		if (strlen(bit)>32){
			printf("Input should be 32 bits...\n\n");
			continue;
		}
		write(sid,(void *)&bit,strlen(bit)+1);
		if (strcmp(bit,"end")==0){
			printf("Client terminated...\n");
			break;
		}
		printf("\nClient sent %s to the Server\n",bit);
		read(sid,ans,MAX);
		printf("Bit-Stuffed Result received from the server : %s\n\n",ans);
	}
	close(sid);
	return 0;
}
