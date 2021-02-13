#include <stdio.h>
#include <netdb.h>
#include <fcntl.h>
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

int main(){
    struct sockaddr_in servaddr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0){
        printf("Error in socket creation!\n");
        exit(1);
    }


    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");


    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        printf("Connection Error!\n");
        exit(1);
    }

    char buffer[MAX];

    read(sockfd, buffer, sizeof(buffer));

    int fd = open(buffer, O_RDONLY);
    bzero(buffer, sizeof(buffer));
    read(fd, buffer, sizeof(buffer));
    close(fd);
    
    printf("Data read from file: %s\n", buffer);

    char code[MAX];
    strcpy(code, buffer);
    strrev(code);
    int code_len = strlen(code);

    char error[MAX] = {0};

    int pos = 0; //Position to check in binary value
    int index = 0;
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
            error[index++] = ctr%2? '1':'0';
        }
    }
    error[index] = 0;
    int error_index = 0;
    for(int i = 0; error[i]; i++)
        if (error[i] == '1')
            error_index += (int)pow(2, i);
    
    if(error_index == 0){
        printf("No error detected!\n");
        
    }
    else{
        printf("Error detected at position : %d\n", error_index);
        if(code[error_index-1] == '1')
            code[error_index-1] = '0';
        else
            code[error_index-1] = '1';
    }

    printf("Extracted data: ");
    char data[MAX] = {0};
    int ix = 0;
    for(int i = 0;i<code_len;i++){
        int ham_bit = 0;
        for(int j = 0; j < code_len && !ham_bit; j++){
            if((i+1) == pow(2, j))
                ham_bit = 1;
        }
        if(ham_bit)
            continue;
        
        data[ix++] = code[i];
    }
    data[ix] = 0;
    strrev(data);
    printf("%s\n", data);

    close(sockfd);
}