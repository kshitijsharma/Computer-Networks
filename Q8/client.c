#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#define PORT 8080
#define MAX 1024
#define SA struct sockaddr
int validate(char *const address)
{
char addr_copy[30];
strcpy(addr_copy, address);
char *split;
int val;
split = strtok(addr_copy, ".");
while (split)
{
val = atoi(split);
if (val < 0 || val > 255)
return -1;
split = strtok(NULL, ".");
}
return 0;
}
int main()
{
int sockfd, n = 0;
struct sockaddr_in servaddr, cliaddr;
char fname[1000];
char buff[1024];
sockfd = socket(AF_INET, SOCK_STREAM, 0);
if(sockfd == -1){
printf("socket creation falied..\n");
exit(0);
}
else{
printf("Socket successfully created.\n");
}
bzero(&servaddr, sizeof(servaddr));
servaddr.sin_family = AF_INET;
servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
servaddr.sin_port = htons(PORT);
if(connect(sockfd,(SA*)&servaddr,sizeof(servaddr))!=0){
printf("Connection with the server failed..\n");
exit(0);
}
else{
printf("Connected to the server..\n");
}
char new[100];
while(1){
bzero(&buff,sizeof(buff));
printf("\nEnter the IP Address ");
scanf("%[^\n]", buff);
int i=0;
bzero(&new,sizeof(new));
while (buff[i] != ' '){
	new[i] = buff[i];
	i++;
}
new[i]='\0';
int v = validate(new);
if (v == 0)
	break;
printf("IP Address is not Valid. Try again.\n");
}
//n = write(sockfd, buff, sizeof(buff));
char new_path[100];
printf("Save the file in path: ");
scanf("%s", new_path);
int fd = creat(new_path, S_IRWXU);
//n = read(sockfd, buff, sizeof(buff));
write(fd, buff, strlen(buff));
bzero(&buff,sizeof(buff));
strcpy(buff,new_path);
n = write(sockfd, buff, sizeof(buff));
printf("File transferred\n");

bzero(&buff,sizeof(buff));
n = read(sockfd,buff,sizeof(buff));
printf("Default Mask from server: %s \n",buff);
close(sockfd);
close(fd);
return 0;
}
