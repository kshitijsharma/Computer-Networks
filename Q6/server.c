#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

void byteUnstuffing(char stuffed[], char unstuffed[])
{
	int i, j;

	for(i=0, j=0; i<strlen(stuffed); i++,j++)
	{

		if(stuffed[i] == 'e')
		{
			if(stuffed[i+1] == 'e' || stuffed[i+1] == 'f')
			{
				i++;
			}
		}

		unstuffed[j] = stuffed[i];

	}

	unstuffed[j]='\0';
}

void byteStuff(int sockfd)
{
	char buff[MAX];

	char flag = 'f';
	char unstuffed[MAX];

	bzero(buff, MAX);

	read(sockfd, buff, sizeof(buff));

	printf("\nCLIENT: %s",buff);

	printf("\n\nBEFORE Byte UNstuffing : %s", buff);

	byteUnstuffing(buff, unstuffed);
	printf("\nAFTER Byte UNstuffing : %s", unstuffed);

	printf("\nBEFORE FLAGS :  %s ", unstuffed);
	printf("\nAFTER FLAGS  : %c%s%c", flag,unstuffed,flag);

	bzero(buff, MAX);

}

int main()
{
	int sockfd, connfd;
	socklen_t len;
	struct sockaddr_in servaddr, cli;


	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		printf("Socket creation failed !\n");
		exit(0);
	}
	else
		printf("Socket successfully created\n");

	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);


	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0)
	{
		printf("Socket bind failed !\n");
		exit(0);
	}
	else
		printf("Socket successfully binded\n");


	if ((listen(sockfd, 5)) != 0)
	{
		printf("Listen failed !\n");
		exit(0);
	}
	else
		printf("Server listening\n");
	len = sizeof(cli);

	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("Server acccept failed !\n");
		exit(0);
	}
	else
		printf("Server acccepted the client\n");

	byteStuff(connfd);

	printf("\n\nSERVER Exit !\n");

	close(sockfd);
}
