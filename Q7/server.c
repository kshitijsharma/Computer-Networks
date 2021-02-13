
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT	 8080 
#define MAXLINE 1024 

typedef struct table{
	char data[10];
	int flag;
	int frameno;
}node;

int main() { 
	int sockfd; 
	char buffer[MAXLINE]; 
	node details;
	struct sockaddr_in servaddr, cliaddr; 
	 char *ack = "ACK";
	 char *noack="NACK";
	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(PORT); 
	
	// Bind the socket with the server address 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
			 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	int len, n; 

	len = sizeof(cliaddr); 
	for(int i=0;i<5;i++){
		printf("\n");
		n = recvfrom(sockfd, &details, sizeof(node),MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len); 
		printf("\nData: ");
		printf("%s",details.data);
		printf("\nFrame number");
		printf("%d",details.frameno);
		if(details.flag==0){
			printf("\n%s",ack);
			printf("\n");
			sendto(sockfd, (const char *)ack, strlen(ack), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
		}
		else if(details.flag==1){
			printf("\n%s",noack);
			sendto(sockfd, (const char *)noack, strlen(noack), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);  
		}
	}
			
	
	
	return 0; 
} 
