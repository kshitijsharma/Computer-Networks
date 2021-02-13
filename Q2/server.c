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
#include<fcntl.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

int valid(char * address)
{
	char temp[100];
	strcpy(temp, address);

	int n = strlen(address);
	if(n != 17)
		return 1;

	for(int i =0; i<17; ++i){
		if((temp[i] >= '0' && temp[i] <= '9' )||(temp[i] >= 'A' && temp[i] <= 'F'));
		else if ((temp[i] == '-' || temp[i] == ':') && (i+1)%3==0);
		else return 1;
	}

	return 0;
}

void startadd(int ip[4], int prefix){

	int q,r,num;
	q = prefix/8;
	r = prefix%8;
	for (int i = 0; i < 4; ++i)
	{
		if(i==q){
			num = ip[i];
			num = num >> (8-r);
			num = num << (8-r);
			ip[i]= num;
		}
		else if(i>q)
			ip[i]=0;
	}
}

void lastadd(int ip[4], int prefix){

	int q,r,num;
	q = prefix/8;
	r = prefix%8;
	for (int i = 0; i < 4; ++i)
	{
		if (i>q)
			ip[i]=255;
		else if(i==q){
			num = ip[i];
			num = num >> (8-r);
			num = num << (8-r);
			ip[i]= num + pow(2,(8-r)) - 1;
		}
	}
}


int main(int argc, char **argv)
{
	int len;
	int sockfd, newfd, n;
	struct sockaddr_in servaddr, cliaddr;
	char buff[1024];
	char str[1000];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		perror("Cannot create socket!\n");

	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(7229);

	if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
		perror("Bind error!\n");
	printf("Waiting for client\n");

	listen(sockfd,2);

	len = sizeof(cliaddr);

	newfd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);

	read(newfd, buff, sizeof(buff));
	printf("Received IP address: %s\n", buff);

	int ip[6], prefix,i=0;
	char *split;
	split = strtok(buff, ".");
	while(split)
	{
		ip[i]=atoi(split);
		i++;
		split = strtok(NULL, ".");
	}

	prefix = 26;

	startadd(ip, prefix);

	printf("\nStart Address : %d.%d.%d.%d\n",ip[0],ip[1],ip[2],ip[3]);

	char addr[128];
	int index = 0;
	for (i=0; i<4; i++)
	{
		index += sprintf(&addr[index], "%d", ip[i]);
   		addr[index++] = '.';
	}
   	addr[index]='\0';

	lastadd(ip, prefix);

	printf("\nLast Address : %d.%d.%d.%d\n\n",ip[0],ip[1],ip[2],ip[3]);
	char addr2[128];
	index = 0;
	for (i=0; i<4; i++)
	{
		index += sprintf(&addr2[index], "%d", ip[i]);
   		addr2[index++] = '.';
	}
   	addr2[index]='\0';
   	strcat(addr,"\n");
   	strcat(addr,addr2);


	//char pre[3];
	
	// for(i=0; buff[i] != '\0'; i++)
	// {
	// 	if(buff[i] == '/')
	// 	{
	// 		pre[0]=buff[i+1];
	// 		pre[1]=buff[i+2];
	// 		break;
	// 	}
	// }
	// prefix = atoi(pre);
	//printf("Prefix: %d\n", prefix);


	int fd = creat("sample.txt", O_WRONLY);

	bzero(buff,sizeof(buff));
	strcpy(buff,addr);
	write(fd, buff, strlen(buff));
	write(newfd,buff,sizeof(buff));
	printf("File sent\n\n");

	char mac[100];
	bzero(buff,sizeof(buff));
	read(newfd, buff, sizeof(buff));
	printf("Received MAC address: %s\n", buff);
	strcpy(mac,buff);
	if(valid(buff) == 0)
	{
		printf("Mac valid\n");
		strcpy(str,"Mac valid");
	}
	else
	{
		printf("Mac invalid\n");
		strcpy(str,"Mac invalid");
	}

	fd = open("sample.txt",O_WRONLY|O_APPEND);
	bzero(buff,sizeof(buff));
	strcpy(buff,"\n");
	strcat(buff,mac);
	strcat(buff," ");
	strcat(buff,str);
	write(fd, buff, strlen(buff));
	write(newfd,buff,sizeof(buff));
	printf("File sent");


	close(fd);
	close(sockfd);
	close(newfd);
	
}

