#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h> 
#include<arpa/inet.h>
#include<unistd.h>
#include<math.h>
#include<stdlib.h>

typedef struct node{
    int sqn;
    int sbt, ebt; //Start and end byte number
    char data[1000];
} segment;

int main(int argc,char *argv[]){
    int sockid,new_fd;
    sockid=socket(AF_INET,SOCK_STREAM,0);
    int option=1;
    setsockopt(sockid,SOL_SOCKET, SO_REUSEADDR,&option,sizeof(option));
    if(sockid<0)
        {
            printf("\nSocket not created");
            return -1;
        }
    printf("\nSuccesful socket created");
    struct sockaddr_in server_addr,client_addr;
    char message[1024];

    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    server_addr.sin_port=htons(7228);
    connect(sockid,(struct sockaddr *)&server_addr,sizeof(server_addr));
    printf("\nSuccesfully connected to the server");
            
   
    int n;
    
    int isn=(rand()%(10-0+1))+0;
    printf("\nInitial Sequence number : %d",isn);

    //Send the initial sequence number to the server
    segment initial;
    initial.sqn=isn;
    initial.sbt=0; initial.ebt=0;

    int startnum=isn;

   

    int i=-1;
    
    while(1){


        if(i==-1)
        {
        strcpy(initial.data,"");
        n=write(sockid,(struct node*)&initial,sizeof(initial));
        i++;
        continue;
        }   
        if(i==3)
            break;

        segment info;
        info.sqn=startnum;
        info.sbt=startnum;
        strcpy(info.data,"Data :- ");
        char c=i+49;
        info.data[strlen(info.data)]=c;
        info.data[strlen(info.data)+1]='\0';
        info.ebt=info.sbt+sizeof(info.data)-1;

        printf("\nSending segment: %d, initial byte number: %d",info.sqn, info.sbt);

        n=write(sockid,(struct node*)&info,sizeof(info));
        segment ackldg;
        n=read(sockid,(struct node*)&ackldg,sizeof(ackldg));
        printf("\nAcknowlegment from the server: %d",ackldg.sqn);

        startnum=ackldg.sqn;
        i++;

    }

    close(sockid);
    printf("\n");
    return 0;
}
