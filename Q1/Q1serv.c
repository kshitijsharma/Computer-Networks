#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<netdb.h>
#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
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


int checksumvalidate(long int csum,char s1[200],char s2[200]){
    int s=0,val=0;
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
   val=s+csum+1;
   //printf("%x\n",val );
   if(val==0){return -1;}
   else return 1;

}



int main(int argc,char **argv)
{
int sockfd;
char srcip[200],srcmac[200],dip[200];
int no;long int checks;
socklen_t len;
//char msg[1024];
char mymac[200];
struct sockaddr_in servaddr,cliaddr;
sockfd=socket(AF_INET,SOCK_DGRAM,0);
packet* p=(packet*)malloc(sizeof(packet));
bzero(&servaddr,sizeof(servaddr));
servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=INADDR_ANY;
servaddr.sin_port=htons(5035);
strcpy(mymac,"12:C0:EE:14:4F:FF");
printf("\n\n [+]Binded");
bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
printf("\n\n [+]Listening...");
for(;;)
{
      printf("\n ");
     len=sizeof(cliaddr);
     no=recvfrom(sockfd,p,sizeof(packet),0,(struct sockaddr*)&cliaddr,&len);
     printf("\n Client%d: ",p->n);
      printf("%s||%s||%s||%s\n",p->sip,p->smac,p->destip,p->data);
     strcpy(p->destmac,mymac);
       checks=p->cksum;
       strcpy(srcip,p->sip);strcpy(srcmac,p->smac);strcpy(dip,p->destip);
     printf("\nChecksum validation...\n");

     
     checks=checksumvalidate(checks,srcip,srcmac);
     
     if(checks){printf("no err!\n");}
     else printf("err present!\n");
     
    
     
   
     if(no<6)
        perror("send error");
     sendto(sockfd,p,sizeof(packet),0,(struct sockaddr*)&cliaddr,len);
     //printf("\n\n[+]Echoed back to the client!\n");
}
return 0;
}
