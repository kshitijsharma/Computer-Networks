#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#define PORT 8080
#define MAX 1024
#define SA struct sockaddr
void findmask(char mask[],int n)
{
	for(int i = 0;i<32;i++){
		if(i<n)
			mask[i] = '1';
		else
			mask[i] = '0';
	}
	mask[32]='\0';
}
void string(char string[],int n)
{
	int j = 0;
	int temp  = n;
	while(temp != 0){
		j++;
		temp /= 10;
	}
	string[j--] = '\0';
	while(j != -1){
		string[j--] = (n%10) + '0';
		n /= 10;
	}
	
}
char findclass(char *const address){
	char addr_copy[30];
	strcpy(addr_copy, address);
	char *split;
	int val;
	split = strtok(addr_copy, ".");
	val = atoi(split);
	if (val<128)
		return 'a';
	else if (val<192)
		return 'b';
	else if (val<224)
		return 'c';
	else if (val<240)
		return 'd';
	else 
		return 'e';
}
int main(int argc, char **argv)
{
int len;
int sockfd, newfd, n;struct sockaddr_in servaddr, cliaddr;
char buff[1024];
char str[1000];
sockfd = socket(AF_INET, SOCK_STREAM, 0);
printf("Server Socket is created.\n");
if(sockfd < 0)
perror("Cannot create socket!\n");
bzero(&servaddr, sizeof(servaddr));
servaddr.sin_family = AF_INET;
servaddr.sin_addr.s_addr = INADDR_ANY; //htonl("127.0.0.1");
servaddr.sin_port = htons(PORT);
if((bind(sockfd,(SA*)&servaddr,sizeof(servaddr)))!=0){
printf("Socket bind failed..\n");
exit(0);
}
printf("Socket successfully binded..\n");
if(listen(sockfd, 5) == 0){
printf("Listening....\n");
}
else{
printf("Error in binding.\n");
}
len = sizeof(cliaddr);
newfd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);
n = read(newfd, buff, sizeof(buff));
printf("File name: %s\n", buff);
int i = 0;
char new[100];
int fd = open(buff, O_RDONLY);
if(fd < 0)
strcpy(buff, "Does not exist!\n");
else{
n = 0;
bzero(buff, sizeof(buff));
read(fd, buff, sizeof(buff));
close(fd);
}
bzero(&new,sizeof(new));
while (buff[i] != ' '){
	new[i] = buff[i];
	i++;
}
new[i++]='\0';
i++;
char num[3];
int j = 0;
num[0] = buff[i++];
num[1] = buff[i++];
num[2] = '\0';
int no = (int) strtol(num,NULL,10);
char a = findclass(new);
printf("IP %s belongs to class %c \n",new,a);
char classmask[40];
switch(a){
case 'a': strcpy(classmask,"255.0.0.0");break;
case 'b': strcpy(classmask,"255.255.0.0");break;
case 'c': strcpy(classmask,"255.255.255.0");break;
case 'd': strcpy(classmask,"No Default Subnet(Multicasting)");break;
case 'e': strcpy(classmask,"No Default Subnet(Future)");break;
default: break;
}
printf("Default Mask for the Class: %s\n" ,classmask);
printf("Given Mask : %d\n",no);
char mask[30];
findmask(mask,no);
char mask1[10];
j = 0;
int temp;
char numtemp[5];
char output[1024];
bzero(output,25);
for(i = 0;i<32;i++){
	mask1[j++] =mask[i];
	if(j == 8){
		mask1[j] = '\0';
		temp = (int)strtol(mask1,NULL,2);
		bzero(numtemp,5);
		string(numtemp,temp);
		j = 0;
		if(output[0]){
			strcat(output,".");
		}
		strcat(output,numtemp);
	}
}
n = write(newfd, output, sizeof(output));
close(newfd);
close(sockfd);return 0;
}
