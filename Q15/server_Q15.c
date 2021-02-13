#include<stdio.h>
#include <unistd.h> 
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#define PORT 7288
#define MAX 10000

int validate(char mac[MAX]){
	int i=0;int count=0;
	char *r1;
	r1=strtok(mac,":");
	while(r1!=NULL){
		count=count+1;
		r1=strtok(NULL,":");
	}
	if(count==6){return 1;}
	else return 0;
}

int mactype(char mac[MAX]){
	int i=0,count=0;char *r1;
	char val[100]="";
	if(strcmp(mac,"FF:FF:FF:FF:FF:FF")==0){return 1;}
	else{
		r1=strtok(mac,":");
		while(r1!=NULL && count<3){
			strcat(val,r1);
			r1=strtok(NULL,":");
			count=count+1;
		}//printf("%s\n",val );
		if(strcmp(val,"01005E")==0){return -1;}
		else if(strcmp(val,"01005E")!=0){return 3;}
		
	}
}


int main(int argc,char **argv){
int sockfd,newfd;int i,len;int val,v;
char buffer[MAX];char f,a;char mssg[MAX];char smac[MAX];
struct sockaddr_in server,client;
sockfd=socket(AF_INET,SOCK_STREAM,0);
if(sockfd<0){printf("socket creation failure!");}
else printf("socket created");
bzero(&server,sizeof(server));
server.sin_family=AF_INET;
server.sin_addr.s_addr=INADDR_ANY;
server.sin_port=htons(PORT);
if(bind(sockfd,(struct sockaddr_in*)&server,sizeof(server))<0){printf("bind error");}
else printf("bind successfully");
listen(sockfd,2);
len=sizeof(client);
newfd=accept(sockfd,(struct sockaddr_in*)&client,&len);
bzero(&buffer,sizeof(buffer));
/*i=0;
while(a=getchar()!='\n'){
buffer[i]=a;
i++;
}*/
while(1){
	printf("\nServer:");
gets(mssg);

strcpy(buffer,mssg);
write(newfd,buffer,sizeof(buffer));
bzero(&buffer,sizeof(buffer));

read(newfd,buffer,sizeof(buffer));
printf("\nClient%c:%s",buffer[0],buffer+1 );
printf("Validate MAC...\n");
strcpy(smac,buffer+1);printf("MAC VALUE received :%s\n",smac );
val=validate(smac);
if(val==1)printf("Valid MAC\n");
else printf("INvalid MAC\n");
strcpy(smac,buffer+1);
v=mactype(smac);
if(v==1)printf("Broadcast MAC\n");
else if(v==-1)printf("Multicast MAC\n");
else printf("UNicast MAC\n");
if(strcmp(buffer+1,"goodbye")==0){break;}
bzero(&buffer,sizeof(buffer));

}
close(newfd);
close(sockfd);
return 0;
}

