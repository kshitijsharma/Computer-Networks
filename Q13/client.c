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



int send_close_req(short int source_port, int portno, int sockfd)
{
  int num_sent, num_recv;
  struct tcp_hdr close_req_seg;
  struct tcp_hdr close_ack_seg;
  char tcp_char_seg[255];

  close_req_seg.src = source_port; //set source port
  close_req_seg.des = portno; //set destination port
  close_req_seg.seq = CLIENT_SEQ; //Assign an initial client sequence number with 
  close_req_seg.ack = 0; //a zero acknowledgement number
  close_req_seg.hdr_flags = FIN; //set the FIN bit to 1
 

  printf("\n\nsent close request\n");
  print_tcp_seg(&close_req_seg);

  memcpy(tcp_char_seg, &close_req_seg, sizeof close_req_seg);

  num_sent = write(sockfd, tcp_char_seg, 255);
  if (num_sent < 0)
  {
    printf("error writing to socket...\n");
    return 1;
  }

  /* read in close acknowledge segment */
  bzero(tcp_char_seg, 255);
  num_recv = read(sockfd, tcp_char_seg, 255); //read from socket
  if(num_recv < 0)
  {
    printf("error reading from socket...\n");
    exit(1);
  }

  memcpy(&close_ack_seg, tcp_char_seg, sizeof close_ack_seg); //copy buffer to segment

  printf("\n\nclose ack received\n");
  print_tcp_seg(&close_ack_seg);

  /* read in second close acknowledge segment */
  bzero(tcp_char_seg, 255);
  num_recv = read(sockfd, tcp_char_seg, 255); //read from socket
  if(num_recv < 0)
  {
    printf("error reading from socket...\n");
    exit(1);
  }

  memcpy(&close_ack_seg, tcp_char_seg, sizeof close_ack_seg); //copy buffer to segment

  close_ack_seg.hdr_flags = ACK;
  close_ack_seg.ack = close_ack_seg.seq;

  printf("\n\nsecond ack received\n");
  print_tcp_seg(&close_ack_seg);

  return 0;
}

int send_final_close_ack(short int source_port, int portno, int sockfd)
{
  int num_sent;
  struct tcp_hdr close_req_seg;
  char tcp_char_seg[255];

  close_req_seg.src = source_port; //set source port
  close_req_seg.des = portno; //set destination port
  close_req_seg.seq = 0; //Assign an initial client sequence number with 
  close_req_seg.ack = 0; //a zero acknowledgement number
  close_req_seg.hdr_flags = FIN; //set the FIN bit to 1
  
  printf("\n\nclose ack sent\n");
  print_tcp_seg(&close_req_seg);

  memcpy(tcp_char_seg, &close_req_seg, sizeof close_req_seg);

  num_sent = write(sockfd, tcp_char_seg, 255);
  if (num_sent < 0)
  {
    printf("error writing to socket...\n");
    return 1;
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

	//demonstrate closing of TCP connection
  	send_close_req(htons(local_addr.sin_port), portno, sockfd);

  	send_final_close_ack((local_addr.sin_port), portno, sockfd); 

	return 0;

}

