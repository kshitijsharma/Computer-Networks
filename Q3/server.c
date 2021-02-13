#include <stdio.h>
#include <netdb.h>
#include<fcntl.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <math.h>
#define MAX 100
#define PORT 8000


void strrev(char* s){
    int len = strlen(s);
    int i = 0;
    int j = len-1;
    while(i<j){
        char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        i++;
        j--;
    }
}

char* conv_to_bin(int number){
    char *bin = (char*)calloc(100, sizeof(char));
    int n = number;
    int pos=0;
    while(n>0){
        bin[pos++] = ('0'+(n%2));
        n /= 2;
    }
    bin[pos] = '\0';
    strrev(bin);
    return bin;
}

int check_position(int number, int position){
    char *bin=(char*)calloc(100, sizeof(char));
    strcpy(bin, conv_to_bin(number));
    int len = strlen(bin);
    return (bin[len - position]=='1')? 1 : 0;
}

int main()
{
    struct sockaddr_in servaddr, cliaddr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0){
        printf("Error in socket creation!\n");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    servaddr.sin_family = AF_INET;

    if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        printf("bind error!\n");
        exit(1);
    }

    if(listen(sockfd, 10) < 0){
        printf("listen error!\n");
        exit(1);
    }

    int len = sizeof(cliaddr);
    int newfd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);

    if(newfd < 0){
        printf("Accept error!\n");
        exit(0);
    }

    char buffer[30], code[30], ip[30];
    printf("Enter the 10bit message: ");
    scanf("%s", buffer);


     int ip_len = strlen(buffer);
    int red_bits = 0;
    for(int i = 0; i<100; i++){
        int lhs = pow(2.0, i);
        int rhs = ip_len + i + 1;
        if( lhs >= rhs){
            red_bits = i;
            break;
        }
    }

    strcpy(ip, buffer);
    strrev(ip);
   
    int code_len = ip_len + red_bits;
    
    //Assign data bits
    int ip_ctr = 0;
    for(int i = 0;i<code_len;i++){
        int ham_bit = 0;
        for(int j = 0; j < code_len && !ham_bit; j++){
            if((i+1) == pow(2, j))
                ham_bit = 1;
        }
        if(ham_bit){
            code[i] = '-';
        }
        else{
            code[i] = ip[ip_ctr];
            ip_ctr++;
        }
    }

    //Hamming code
    int pos = 0; //Position to check in binary value
    for(int i = 0;i<code_len;i++){
        int ham_bit = 0;
        for(int j = 0; j < code_len && !ham_bit; j++){
            if((i+1) == pow(2, j)){
                ham_bit = 1;
                pos = j+1;
            }
        }
        if(ham_bit){
            int ctr = 0;
            for(int j = 0;j<code_len;j++){
                int check_pos = check_position(j+1, pos);
                if(code[j] == '1'&&check_pos){
                    ctr++;
                }
            }
            code[i] = ctr%2? '1':'0';
        }
    }

    strrev(code);
    printf("Number of redundant bits required: %d\n", red_bits);
    printf("Hamming code string: %s\n", code);

    int opt;
    printf("Do you want to introduce error? 1/0 ");
    scanf("%d", &opt);
    if(opt){
        printf("Enter position of error: ");
        scanf("%d", &opt);

        if(code[opt] == '1')
            code[opt] = '0';
        else 
            code[opt] = '1';
    }

    char filename[MAX] = "hamming.txt";

    int fd = creat(filename, S_IRWXU);
    write(fd, code, strlen(code));
    close(fd);

    write(newfd, filename, sizeof(filename));

    close(sockfd);
}