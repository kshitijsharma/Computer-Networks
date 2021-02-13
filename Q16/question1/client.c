#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <unistd.h> //for read write and close
#include <arpa/inet.h> //for inet_addr()

int main(){

	int sockfd,n;
	struct sockaddr_in seraddr;
	char buff[100],cname[20];

	//Socket Creation
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
		perror("Socket Creation failed...!");
	else
		printf("Socket Created Successfully...!\n");

	bzero(&seraddr,sizeof(seraddr));

	seraddr.sin_family=AF_INET;
	seraddr.sin_port=htons(3335);
	seraddr.sin_addr.s_addr=inet_addr("127.0.0.1");

	//Connect to a socket
	connect(sockfd,(struct sockaddr *)&seraddr,sizeof(seraddr));

	//Send message
	printf("Enter Client Name : ");
	scanf(" %[^\n]",cname);
	printf("\n------ Enter IP -----\n");

	while (1)
	{
		bzero(buff, 100);
		printf("%s : ",cname);
		scanf(" %[^\n]",buff);

		write(sockfd, buff, 100);

		if(strcmp(buff, "*") == 0) break;

		read(sockfd, buff, 100);
		printf("Respective MAC : %s\n", buff);
	}
	printf("Connection terminated..!!!\n");
	close(sockfd);

	return 0;
}