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
#include<math.h>


void split(char *addr, int *arr)
{
    int j=0;
    char num[4]; int k=0;
    for(int i=0; addr[i]; i++)
    {
        if(addr[i]=='.' || !addr[i+1])
        {
            arr[k++]=atoi(num);
            bzero(num,sizeof(num));
            j=0;
            continue;
        }

        num[j++]=addr[i];
    }

    // for(int i=0; i<4; i++)
    //     printf(" %d ",arr[i]);
}



void startadd(int ip[4], int prefix){

    int q,r,num;
    q = prefix/8;
    r = prefix%8;
    for (int i = 0; i < 4; ++i)
    {
        if(i==q){
            num = ip[i];
            num = num >> (8-r);
            num = num << (8-r);
            ip[i]= num;
        }
        else if(i>q)
            ip[i]=0;
    }
}

void lastadd(int ip[4], int prefix){

    int q,r,num;
    q = prefix/8;
    r = prefix%8;
    for (int i = 0; i < 4; ++i)
    {
        if (i>q)
            ip[i]=255;
        else if(i==q){
            num = ip[i];
            num = num >> (8-r);
            num = num << (8-r);
            ip[i]= num + pow(2,(8-r)) - 1;
        }
    }
}

int main()
{
    int sockid, new_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];

    sockid=socket(AF_INET, SOCK_STREAM,0);
    int option=1;
    setsockopt(sockid,SOL_SOCKET, SO_REUSEADDR,&option,sizeof(option));
    if(sockid<0)
        {
            printf("\nSocket not created");
            return -1;
        }
    printf("\nSuccesfully socket created");

    bzero(&server_addr,sizeof(server_addr));

    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=INADDR_ANY;
    server_addr.sin_port=htons(7228);
    if(bind(sockid,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
        {
            perror("Bind error");
        }
    printf("\nPort bind successful");

    if(listen(sockid,5)<0)
    {
        printf("\nError in listening");
        return -1;
    }
    printf("\nServer listening");
    int len_c=sizeof(client_addr);

    new_fd=accept(sockid,(struct sockaddr *)&client_addr,&len_c);

    printf("\nServer accepted client\n\n"); 

    int n=read(new_fd,buffer,sizeof(buffer));
    int ip[6], prefix;

    //extracting prefix
    char *split;
    int i=0;
    split = strtok(buffer, "/");
    while(split)
    {
        prefix=atoi(split);
        i++;
        split = strtok(NULL, "/");
    }
    //printf("Prefix: %d\n",prefix);

    //extracting ip
    i=0;
    split = strtok(buffer, ".");
    while(split)
    {
        ip[i]=atoi(split);
        i++;
        split = strtok(NULL, ".");
    }

    printf("\nIP address received from the client is:");
    for(i=0; i<4; i++)
    {
        printf("%d.",ip[i]);
    }
    printf("prefix: %d\n", prefix);

    startadd(ip, prefix);
    printf("\nStart Address : %d.%d.%d.%d\n",ip[0],ip[1],ip[2],ip[3]);

    lastadd(ip, prefix);
    printf("Last Address : %d.%d.%d.%d\n\n",ip[0],ip[1],ip[2],ip[3]);
    // char mask[32];
    // strcpy(mask,"");

    // int nmask[4];
    // find_mask(subnet,mask,nmask);

    // first_last(arr,nmask);


    printf("\n");
    close(new_fd);
    close(sockid);
    return 0;




}