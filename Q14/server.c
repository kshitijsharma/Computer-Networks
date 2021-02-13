#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <unistd.h> //for read, write and close
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>

void subnetmask(int ip[4], int prefix){

	int q,r,num;
	q = prefix/8;
	r = prefix%8;
	for (int i = 0; i < 4; ++i)
	{
		if(i==q){
			num = 255;
			num = num >> (8-r);
			num = num << (8-r);
			ip[i]= num;
		}
		else if(i>q)
			ip[i]=0;
		else
			ip[i]=255;
	}
}

int main(){

	int sockfd,newfd,n,nsubs,ip[4];
	struct sockaddr_in seraddr, client1;
	char buff[100],ipstr[100],type[100],mask[100],*split,temp[100];

	//Socket Creation
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
		perror("Socket Creation failed...!");
	else
		printf("Socket Created Successfully...!\n");

	bzero(&seraddr,sizeof(seraddr));

	seraddr.sin_family=AF_INET;
	seraddr.sin_port=htons(3333);
	seraddr.sin_addr.s_addr=htonl(INADDR_ANY);

	//binding to the socket
	if(bind(sockfd,(struct sockaddr *) &seraddr, sizeof(seraddr))<0)
		perror("Bind failed...!");
	else
		printf("Binded Successfully...!\n");

	//listen for connections
	listen(sockfd,3);

	socklen_t len=sizeof(client1);
	//Accept connection from client
	newfd=accept(sockfd,(struct sockaddr*)&client1,&len);

	//Read client message
	n=read(newfd,ipstr,sizeof(ipstr));
	printf("Recieved network address : %s\n",ipstr);
	strcpy(temp, ipstr);

	n = read(newfd,buff,sizeof(buff));
	nsubs=atoi(buff);
	printf("Recieved No. of subnets : %d\n",nsubs);


    split = strtok(ipstr, ".");
    int val = atoi(split),prefix =0;
    if (val <=127){
           strcpy(type,"A");
           strcpy(mask,"255.0.0.0");
           prefix =8;
        }
    else if (val <=191){
         strcpy(type,"B");
         strcpy(mask,"255.255.0.0");
         prefix =16;
        }
    else if (val <=223){
          strcpy(type,"C");
          strcpy(mask,"255.255.255.0");
          prefix =24;
        }
    else if (val <=239){
          strcpy(type,"D");
          strcpy(mask,"NO MASK");
        }
    else
        {
        strcpy(type,"E");
        strcpy(mask,"NO MASK");
        }
    printf("\nClass type : %s\n", type);
    n=write(newfd,type,sizeof(type));  

    int bits = 0;
    while(pow(2,bits)<nsubs)
    	bits++;
    //printf("\nBITS : %d\n",bits);

    prefix +=bits;
    int i=0;
    split = strtok(ipstr, ".");
	while(split){
		ip[i]=atoi(split);
		i++;
		split = strtok(NULL, ".");
	}

	subnetmask(ip, prefix);
	printf("\nSubnet Mask : %d.%d.%d.%d\n",ip[0],ip[1],ip[2],ip[3]);
	bzero(buff, sizeof(buff));
	sprintf(buff,"Network Address : %s\nSubnet Mask : %d.%d.%d.%d\n",temp,ip[0],ip[1],ip[2],ip[3]);
	printf("%s\n", buff);

	int fd = open("file.txt", O_CREAT | O_WRONLY);
	write(fd, buff, sizeof(buff));

	close(fd);

	fd = open("file.txt", O_RDONLY);
	read(fd, buff, sizeof(buff));

	write(newfd,buff,sizeof(buff));


	close(sockfd);
	close(newfd);

	return 0;
}