/*Write a socket program to perform file transfer.

1st content in file sample.txt.
The client transmits the IP address to the server in the form ‘w.x.y.z ∖n’.
The sever finds the first address and last address for the IP address and writes in the file.
2nd content in file sample.txt.
The client transmits MAC address to the server.
The server validates the MAC address and write the relevant information in file.
The server transfers the sample.txt to the client.
The client stores the sample.txt in its desktop.*/

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<fcntl.h>

int main(int argc, char **argv)
{
	int len, sockfd;
	struct sockaddr_in servaddr, cliaddr;
	char buff[1024];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		perror("Cannot create socket!\n");

	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port = htons(7229);

	connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	bzero(buff,sizeof(buff));
	int ip[6],n;
	printf("Enter IP address: ");
	scanf("%s",buff);


	write(sockfd, buff, sizeof(buff));
	printf("IP address sent\n");

	bzero(buff,sizeof(buff));
	read(sockfd,buff,sizeof(buff));
	int fd = creat("sample1.txt", O_WRONLY);
	write(fd,buff,strlen(buff));
	printf("Received file content: %s\n",buff);

	char mac[100];
	printf("Enter MAC address: ");
	scanf("%s",mac);
	strcpy(buff,mac);

	write(sockfd, buff, sizeof(buff));
	printf("MAC address sent\n");

	bzero(buff,sizeof(buff));
	read(sockfd,buff,sizeof(buff));
	fd = open("sample1.txt",O_WRONLY|O_APPEND);
	write(fd, buff, strlen(buff));
	printf("Received file content: %s\n",buff);

	close(fd);
	close(sockfd);

}