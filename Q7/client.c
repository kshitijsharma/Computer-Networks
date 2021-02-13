
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
node input();
int main() { 
	int sockfd; 
	int number=0;
	int choice;
	char buffer[MAXLINE]; 
	struct sockaddr_in	 servaddr; 

	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	
	int n, len;
	node details[5];
	for(int i=0;i<5;i++){
	
		details[number]=input();
		if(i%2==0)
			details[number].frameno=0;
		else if(i%2!=0)
			details[number].frameno=1;
		number+=1; 
	}
	number=0;
	for(int i=0;i<5;i++){
		printf("\nFor Data %d",i+1);
		printf("\nDo you want to transmit ");
		printf("\n1.with no error");
		printf("\n2.with error");
		scanf("%d",&choice);
		if(choice==1)
			details[number].flag=0;
		else if(choice==2)
			details[number].flag=1;
		sendto(sockfd, &details[number], sizeof(node),MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
			
			
		n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
					
					 
		buffer[n] = '\0'; 
		printf("Server : %s\n", buffer); 
		number+=1;
	}	
	
	close(sockfd); 
	return 0; 
} 

node input(){
	node temp;
	printf("\nEnter the 8 bit data");
	scanf("%s",temp.data);
	return temp;
}