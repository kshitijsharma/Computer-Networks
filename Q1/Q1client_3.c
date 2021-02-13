#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#define MAXLINE 1024
typedef struct packet{
	int n;
	char sip[200];
	char smac[200];
	char destip[200];
	char destmac[200];
	char data[200];
	long int cksum;
	
}packet;


long int checksum(char s1[200],char s2[200]){
	int s=0;
	char  *r,*r1;
	r=strtok(s1,".");
	while(r!=NULL){
		
		//printf("%d\n",atoi(r) );
		s=s+atoi(r);

		r=strtok(NULL,".");
	}
	r1=strtok(s2,":");
	while(r1!=NULL){
		
		//printf("%d\n",atoi(r1) );
		s=s+atoi(r1);
		r1=strtok(NULL,":");
	}
	s=~s;
	return s;
}



int main(int argc,char* argv[])
{
int sockfd;char srcip[200],srcmac[200],dip[200];
int no;
socklen_t len;
packet* p1=(packet*)malloc(sizeof(packet));
//char sendline[1024],recvline[1024];
struct sockaddr_in servaddr;
//strcpy(sendline,"");
printf("\n [+]Enter the message ARP packet : ");
printf("\nsip:" );scanf("%s",p1->sip);strcpy(srcip,p1->sip);
printf("smac:");scanf("%s",p1->smac);strcpy(srcmac,p1->smac);
printf("destip:" );scanf("%s",p1->destip);strcpy(dip,p1->destip);
printf("data:");scanf("%s",p1->data);
p1->n=3;
p1->cksum=checksum(srcip,srcmac);
//strcpy(p1->destmac,"00:00:00:00:00:00");
sockfd=socket(AF_INET,SOCK_DGRAM,0);
bzero(&servaddr,sizeof(servaddr));
servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
servaddr.sin_port=htons(5035);
connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
len=sizeof(servaddr);
sendto(sockfd,p1,sizeof(packet),0,(struct sockaddr*)&servaddr,len);
no=recvfrom(sockfd,p1,MAXLINE,0,NULL,NULL);

printf("\n Server's Echo :\n %s||%s||%s||%s||%s||%ld",p1->sip,p1->smac,p1->destip,p1->destmac,p1->data,p1->cksum);
return 0;
}