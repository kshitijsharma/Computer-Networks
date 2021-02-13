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

typedef char string[10];

typedef struct packet
{
  char data[200];
  long int cksum;

}packet;

int valid_checksum(string data,int csum)
{
  string s1;
  int sum=0;
  for(int i=0;i<16;i=i+8)
  {
    strncpy(s1, data + i, 8);
    s1[8]='\0';
    sum+=strtol(s1,NULL,2);
  }
  sum = sum + csum + 1;
  if(sum == 0)
    return 1;
  else
    return 0;
}

void ChkSum(int sockfd)
{
	packet p;

	bzero(&p, sizeof(p));

	read(sockfd, &p, sizeof(p));

	if(valid_checksum(p.data,p.cksum))
	{
		printf("\nVALID !!!\n");
	}

	else
		printf("\nINVALID !!!\n");

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

	ChkSum(connfd);

	printf("\n\nSERVER Exit !\n");

	close(sockfd);
}
