#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

void byteStuffing(char data[], char stuffed[])
{
	int i, j;

	for(i=0, j=0; i<strlen(data); i++,j++)
	{

		if(data[i] == 'f' || data[i] == 'e')
		{
			stuffed[j++] = 'e';
		}

		stuffed[j] = data[i];
	}

	stuffed[j] = '\0';
}

void byteStuff(int sockfd)
{
	char buff[MAX];

	char data[MAX];
	char stuffed[MAX];

	bzero(buff, sizeof(buff));
	printf("\nf:flag d:data e:esc\n\n");
	printf("\nEnter DATA: ");				//dfeddfed
	scanf("%s",data);

	printf("\n\nBEFORE Byte Stuffing: %s",data);

	byteStuffing(data, stuffed);
	printf("\nAFTER Byte stuffing : %s", stuffed);

	strcpy(buff, stuffed);

	write(sockfd, buff, sizeof(buff));
	bzero(buff, sizeof(buff));

}

int main()
{
	int sockfd;
	struct sockaddr_in servaddr;

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
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("Connection to the server failed !\n");
		exit(0);
	}
	else
		printf("Connected to the server\n");

	byteStuff(sockfd);

	printf("\n\nCLIENT Exit !\n");

	close(sockfd);
}

