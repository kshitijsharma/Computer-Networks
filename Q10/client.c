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

typedef char string[10];

typedef struct packet
{
  char data[200];
  long int cksum;

}packet;

int checksum(string data)
{
  string s1;
  int sum=0;
  for(int i=0;i<16;i=i+8)
  {
    strncpy(s1, data + i, 8);
    s1[8]='\0';
    sum+=strtol(s1,NULL,2);
  }
  sum = ~sum;
  return sum;
}

void ChkSum(int sockfd)
{
	packet p;
	int pos = 0;

	printf("Enter Binary : ");
	//strcpy(p.data, "1011000011110101");
	scanf("%s",p.data);

	p.cksum=checksum(p.data);

	printf("\n\nDATA : %s",p.data);
	printf("\nEnter Position to introduce Error : \n");
	printf("(0 for no Error) : ");
	scanf("%d",&pos);
	printf("\nOLD DATA : %s",p.data);

	if(pos!=0 && pos <17)
	{
		pos = pos - 1;

		if(p.data[pos]=='1')
		  p.data[pos]='0';
		else
		  p.data[pos]='1';
	}

	printf("\nNEW DATA : %s",p.data);

	write(sockfd, &p, sizeof(p));

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

	ChkSum(sockfd);

	printf("\n\nCLIENT Exit !\n");

	close(sockfd);
}

