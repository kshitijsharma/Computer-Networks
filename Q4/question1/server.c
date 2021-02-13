#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <unistd.h> //for read, write and close
#include <stdlib.h>

#define MAX 4

int main()
{

    int sockfd;
    struct sockaddr_in seraddr, cli;
    socklen_t n;
    char buff[100];

    char ip[10][50]={"192.168.72.0","192.128.45.8","192.133.47.0","192.134.48.7"};
    char mac[10][50]={"","","",""};
    char duration[10][50]={"1.1","2.2","3.3","4.4"};

    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0)
        perror("Socket Creation failed...!");
    else
        printf("Socket Created Successfully...!\n");

    bzero(&seraddr,sizeof(seraddr));

    seraddr.sin_family=AF_INET;
    seraddr.sin_port=htons(3335);
    seraddr.sin_addr.s_addr=htonl(INADDR_ANY);

    //binding to the socket
    if(bind(sockfd,(struct sockaddr *) &seraddr, sizeof(seraddr))<0)
       perror("Bind failed...!");
    else
       printf("Bound Successfully...!\n");


    n=sizeof(cli);

   
    while(1)
    {
        recvfrom(sockfd, buff, sizeof(buff), MSG_WAITALL, (struct sockaddr*)&cli, &n);
        printf("MAC address of client: %s\n", buff);

        int flag = 0;
        for(int i =0; i < 4; i++){
            if(mac[i][0] == 0)
            {
                strcpy(mac[i], buff);
                strcpy(buff, ip[i]);
                flag = 1;
                break;
            }
        }
        if(flag == 1){
            printf("Allocated IP Address: %s\n",buff);
            sendto(sockfd, buff, sizeof(buff), MSG_CONFIRM, (struct sockaddr*)&cli, n);
        }

        printf("IP ADDRESS\t\tMAC ADDRESS\t\tTIME\n");
        for(int i = 0; i < 4;i++)
            printf("%s\t\t%s\t\t%s\n", ip[i], mac[i], duration[i]);
        printf("\n\n");

    }


    close(sockfd);
    return 0;
    }
