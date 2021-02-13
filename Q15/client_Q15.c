#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>
#include <unistd.h> 
#define PORT 7288
#define MAX 10000
int main(){
	int sockfd;
	char buff[MAX],mssg[MAX];
	struct sockaddr_in server,client;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0){printf("socket creation failed");}
	else printf("socket created successfully");
	bzero(&server,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	server.sin_port=htons(PORT);
	connect(sockfd,(struct sockaddr_in*)&server,sizeof(server));
	while(1){
	read(sockfd,buff,sizeof(buff));
	printf("\nServer:%s",buff);
	printf("\nClient:");
	gets(mssg);
	
	bzero(&buff,sizeof(buff));
	buff[0]='1';
	strcat(buff,mssg);

	write(sockfd,buff,sizeof(buff));
	if(strcmp("goodbye",mssg)==0){break;}
	bzero(&buff,sizeof(buff));
}
	close(sockfd);
	return 0;
}
	

