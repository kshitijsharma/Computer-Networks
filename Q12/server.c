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

    char *code;
    int sockid,len_c,new_fd;
    char message[1024],data[1024];
    

    sockid=socket(AF_INET,SOCK_STREAM,0);
    int option=1;
    setsockopt(sockid,SOL_SOCKET, SO_REUSEADDR,&option,sizeof(option));
    if(sockid<0)
        {
            printf("\nSocket not created");
            return -1;
        }
    printf("\nSuccesfully socket created");
    struct sockaddr_in server_addr,client_addr,clients_addr[5];
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=INADDR_ANY;
    server_addr.sin_port=htons(7228);
    if(bind(sockid,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
        {
            perror("Bind error");
        }
    printf("\nPort bind successful");
    listen(sockid,5);
    printf("\nServer listening");
    len_c=sizeof(client_addr);
    int clients[5],client_len=0;
    new_fd=accept(sockid,(struct sockaddr *)&client_addr,&len_c);
    printf("\nServer accepted client");    

    int isn,n,acknum;
    segment initial;
    

    int i=-1;
    
    while(1)
    {   

        
        if(i==-1)
        {
            n=read(new_fd,(struct node*)&initial,sizeof(initial));

            isn=initial.sqn;
            printf("\nInitial Sequence number recieved from client: %d",isn);
            acknum=isn;
            i++;
            continue;
        }

          if(i==3)
            break;

        segment info;
        n=read(new_fd,(struct node*)&info,sizeof(info));


        if(info.sbt!=acknum)
        {
            printf("\nError in data");
            return 0;
        }

        printf("\nRecieved: %s",info.data);
        printf("\nSegment number: %d, final byte number: %d",info.sqn,info.ebt);
        acknum=info.ebt+1;
        segment ackldg;
        ackldg.sqn=acknum;
        ackldg.ebt=0;
        ackldg.sbt=0;
        strcpy(ackldg.data,"");
        n=write(new_fd,(struct node*)&ackldg,sizeof(ackldg));
        i++;
      
        
    }

    close(new_fd);
    close(sockid);
  
    printf("\n");
    return 0;

}