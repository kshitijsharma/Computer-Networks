#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <sys/time.h>
#include <netinet/tcp.h>


int checkvalid(char *ip){
    char num[100];
    for(int i=0,k=0; ip[i]; i++)
    {
        if(ip[i]=='.' || !ip[i+1])
            {
                if(!ip[i+1])
                    num[k++]=ip[i];
                num[k]='\0';
                int n=atoi(num);
                if (!(n>0 && n<255))
                {
                    printf("\nInvalid IP address");
                    return 0;
                }
                k=0;
                memset((char *)num,0,sizeof(num));
                continue;

            }
        num[k++]=ip[i];
    }


    return 1;
}


int main()
{
    int sockid;
    struct sockaddr_in server_addr;
    char buffer[1024];
    sockid=socket(AF_INET,SOCK_STREAM,0);
    int option=1;
    setsockopt(sockid,SOL_SOCKET, SO_REUSEADDR,&option,sizeof(option));
    if(sockid<0)
        {
            printf("\nSocket not created");
            return -1;
        }

    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    server_addr.sin_port=htons(7228);
    connect(sockid,(struct sockaddr *)&server_addr,sizeof(server_addr));
    printf("\nSuccesfully connected to the server");
  int v=0;
  int k=1;

    char ip[1024]; int i;
   
   while(v==0)
   {    

       if(k==0)
        printf("\nInvalid IP address");
    k=1;
    
       printf("\nEnter IP: ");
        scanf(" %[^\n]",buffer);

        for( i=0; buffer[i+3]; i++)
        {
            ip[i]=buffer[i];
        }
        ip[i]='\0';
        v=checkvalid(ip);

   }

   printf("\nValid ip : %s",ip);

    int status=write(sockid,buffer,sizeof(buffer));    




   

    printf("\n");
    // close(sockid);
    return 0;

}