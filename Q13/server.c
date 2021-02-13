#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#define PORT_NO 6000
#define SERVER_SEQ 200

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

int recv_conn_req(int accept_sock)
{
	int num_data_recv, num_sent, temp_portno;
	char buffer[255];
	struct tcp_hdr tcp_seg;

	num_data_recv = read(accept_sock, buffer, 255);

	if(num_data_recv < 0)
	{
		printf("error receiving from socket\n");
		exit(1);
	}

	memcpy(&tcp_seg, buffer, sizeof(tcp_seg));

	printf("\n\nconn req from client\n");
	print_tcp_seg(&tcp_seg);	

	//set SYN bit to and ACK bit to 1
	tcp_seg.hdr_flags = (SYN | ACK);

	tcp_seg.ack = tcp_seg.seq + 1; 
	tcp_seg.seq = SERVER_SEQ; 
	
	temp_portno = tcp_seg.src;
	tcp_seg.src = tcp_seg.des;
	tcp_seg.des = temp_portno; 
	

  	printf("\n\nconn accept to client\n");
  	print_tcp_seg(&tcp_seg);

	memcpy(buffer, &tcp_seg, sizeof tcp_seg);	
	num_sent = write(accept_sock, buffer, 255); 

	if (num_sent < 0)
	{
	  printf("error writing to socket...\n");
	  exit(1);
	}	
	return 0;
}

int recv_close_req(int accept_sock)
{
	int num_data_recv, num_sent, temp_portno;
	char buffer[255];
	struct tcp_hdr tcp_close_seg;

	num_data_recv = read(accept_sock, buffer, 255);
	if (num_data_recv < 0)
	{
		printf("error receiving data from socket\n");
		exit(1);
	}

	memcpy(&tcp_close_seg, buffer, sizeof tcp_close_seg);

	printf("\n\nreceived close request\n");
	print_tcp_seg(&tcp_close_seg);	

	temp_portno = tcp_close_seg.src;
	tcp_close_seg.src = tcp_close_seg.des;
	tcp_close_seg.des = temp_portno; //change the source and destination to go back to client

	tcp_close_seg.ack = tcp_close_seg.seq + 1;
	tcp_close_seg.hdr_flags = ACK;


	printf("\n\nsent close ack\n");
	print_tcp_seg(&tcp_close_seg);

	memcpy(buffer, &tcp_close_seg, sizeof tcp_close_seg);	//copy segment to char buffer
	num_sent = write(accept_sock, buffer, 255); //send buffer to client
	if (num_sent < 0)
	{
	  printf("error writing to socket...\n");
	  exit(1);
	}		

	tcp_close_seg.hdr_flags = FIN;
	tcp_close_seg.seq = SERVER_SEQ;


	printf("\n\nsent second ack\n");
	print_tcp_seg(&tcp_close_seg);

	bzero(buffer,255);
	memcpy(buffer, &tcp_close_seg, sizeof(tcp_close_seg));	//copy segment to char buffer
	num_sent = write(accept_sock, buffer, 255); //send buffer to client
	if (num_sent < 0)
	{
	  printf("error writing to socket...\n");
	  exit(1);
	}

	return 0;
}


int recv_close_ack(int accept_sock)
{
	int num_data_recv;
	char buffer[255];
	struct tcp_hdr tcp_close_seg;

	num_data_recv = read(accept_sock, buffer, 255);
	if (num_data_recv < 0)
	{
		printf("error receiving data from socket\n");
		exit(1);
	}

	memcpy(&tcp_close_seg, buffer, sizeof tcp_close_seg);

	printf("\n\nreceived close ack\n");
	print_tcp_seg(&tcp_close_seg);	
}


int main(int argc, char **argv)
{
	char buffer[256];
	int num_data_recv, num_sent;	  	
	struct tcp_hdr tcp_seg;
	struct tcp_hdr tcp_ack_seg;

	int serv_sock, clilen;	
	struct sockaddr_in serv_addr;
	
	//create socket
	serv_sock = socket(AF_INET, SOCK_STREAM, 0);

	if(serv_sock < 0)
	{
		printf("Socket could not be opened...\n");
		exit(1);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT_NO);

	if(bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("error while binding...\n");
		exit(1);
	}

	struct sockaddr_in cli_addr;
	socklen_t addr_size;

	int accept_sock;

	listen(serv_sock, 5);
	printf("Listening on port %d\n", PORT_NO);

	addr_size = sizeof(cli_addr);

	accept_sock = accept(serv_sock, (struct sockaddr *)&cli_addr, &addr_size);
	if(accept_sock < 0)
	{
		printf("Error in accepting client\n");
		exit(1);
	}

	//handle the close connection request from client
	recv_close_req(accept_sock);

	recv_close_ack(accept_sock);

	return 0;
}



