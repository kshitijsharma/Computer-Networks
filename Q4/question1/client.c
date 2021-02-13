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
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
		perror("Socket Creation failed...!");
	else
		printf("Socket Created Successfully...!\n");

	bzero(&seraddr,sizeof(seraddr));

	seraddr.sin_family=AF_INET;
	seraddr.sin_port=htons(3335);
	seraddr.sin_addr.s_addr=inet_addr("127.0.0.1");

	n = sizeof(seraddr);

	//Send message
	printf("Enter Client Name : ");
	scanf(" %[^\n]",cname);
	printf("\n------ Enter MAC -----\n");

	
	bzero(buff, 100);
	printf("%s : ",cname);
	scanf(" %[^\n]",buff);

	sendto(sockfd, buff, sizeof(buff), MSG_CONFIRM, (struct sockaddr*)&seraddr, sizeof(seraddr));
	recvfrom(sockfd, buff, sizeof(buff), MSG_WAITALL, (struct sockaddr*)&seraddr, &n);
	printf("Respective IP : %s\n", buff);
	printf("Connection terminated..!!!\n");
	close(sockfd);

	return 0;
}