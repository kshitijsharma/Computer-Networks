#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#define CLIENT_SEQ 100

struct tcp_hdr
{
    short int src;
    short int des;
    int seq;
    int ack;
    short int hlen_res;
    short int hdr_flags;
};


enum 
{
	SYN = 0x01,
	ACK = 0x02,
	FIN = 0x04,
};


void print_tcp_seg(struct tcp_hdr *tcp_seg)
{

	printf("Source port address:\t\t%hu\n", tcp_seg->src);
	printf("Destination port address:\t%hu\n", tcp_seg->des);
	printf("Sequence number:\t\t%d\n", tcp_seg->seq);
	printf("Acknowledgement number:\t%d\n", tcp_seg->ack);
	//printf("Header length:\t\t%d", tcp_seg->hlen);
	
	if(tcp_seg->hdr_flags & SYN)
		printf("Header flag: SYN = 1\n");
	if (tcp_seg->hdr_flags & ACK) 
		printf("Header flag: ACK = 1\n");
	if (tcp_seg->hdr_flags & FIN)
		printf("Header flag: FIN = 1\n");

	printf("Header flags:\t\t0x0%x\n", tcp_seg->hdr_flags);

	return;
}


int send_conn_request(short int source_port, int portno, int sockfd)
{
	int num_sent;
	struct tcp_hdr tcp_seg;
	char tcp_char_segment[255];

	tcp_seg.src = source_port; 
	tcp_seg.des = portno; 
	tcp_seg.seq = CLIENT_SEQ;  
	tcp_seg.ack = 0; 
	tcp_seg.hlen_res = 0;
	tcp_seg.hdr_flags = SYN; //set the SYN bit to 1


	printf("\n\nconnection request sent \n");
	print_tcp_seg(&tcp_seg);

	// send TCP struct to server
	memcpy(tcp_char_segment, &tcp_seg, sizeof(tcp_seg)); 

	num_sent = write(sockfd, tcp_char_segment, 255); 

	if (num_sent < 0)
	{
		printf("error writing to socket...\n");
		exit(1);
	}
	return 0; 
}

int send_ack_tcp_seg(int sockfd)
{
	int num_recv, num_sent, temp_portno;
	char tcp_char_seg[255];
	struct tcp_hdr conn_grant_seg;

	num_recv = read(sockfd, tcp_char_seg, 255);
	if(num_recv < 0)
	{
		printf("error reading from socket...\n");
		exit(1);
	}

	memcpy(&conn_grant_seg, tcp_char_seg, sizeof conn_grant_seg); //copy buffer to segment

	printf("\n\nconnection granted\n");
	print_tcp_seg(&conn_grant_seg);

	temp_portno = conn_grant_seg.src;
	conn_grant_seg.src = conn_grant_seg.des;
	conn_grant_seg.des = temp_portno;

	conn_grant_seg.ack = conn_grant_seg.seq + 1;

	conn_grant_seg.hdr_flags = ACK;

	bzero(tcp_char_seg, 255);
	memcpy(tcp_char_seg, &conn_grant_seg, sizeof conn_grant_seg);

	printf("\n\nack sent\n");
	print_tcp_seg(&conn_grant_seg);

	num_sent = write(sockfd, tcp_char_seg, 255); //write back to server
	if (num_sent < 0)
	{
		printf("error writing to socket...\n");
		exit(1);
	}

	return 0;
}


int main(int argc, char **argv)
{
	int portno, sockfd;
	socklen_t local_addr_len;
	int num_attempts = 0;	
	struct sockaddr_in local_addr, serv_addr;
	struct hostnet *server;  
	struct tcp_hdr conn_grant;   

	if (argc < 2) 
	{
		printf("Usage %s Port_Number \n", argv[0]);
		exit(0);
	}

	portno = atoi(argv[1]);	//set port number from user input

	//create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0 )
	{
		printf("Error creating socket\n");
		exit(1);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //IP for localhost
	serv_addr.sin_port = htons(portno);

	/* Connecting to the server */
	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) 
	{
		printf("ERROR while attempting to  connect");
		exit(1);
	} 

	// get source port
	local_addr_len = sizeof(local_addr);
	getsockname(sockfd, (struct sockaddr *)&local_addr, &local_addr_len);  

	//send the connection request
	send_conn_request(htons(local_addr.sin_port), portno, sockfd);

	//receive response and send acknowledgement tcp segment
	send_ack_tcp_seg(sockfd);  

	return 0;

}

