// CHECKSUM DECIMAL VERSION
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef char string[10];

typedef struct packet{
  char data[200];
  long int cksum;

}packet;

int checksum(string data)
{
  string s1;
  int sum=0;
  for(int i=0;i<16;i=i+8)
  {
    strncpy(s1, data + i, 8);
    s1[8]='\0';
    sum+=strtol(s1,NULL,2);
  }
  sum = ~sum;
  return sum;
}

int valid_checksum(string data,int csum)
{
  string s1;
  int sum=0;
  for(int i=0;i<16;i=i+8)
  {
    strncpy(s1, data + i, 8);
    s1[8]='\0';
    sum+=strtol(s1,NULL,2);
  }
  sum = sum + csum + 1;
  if(sum == 0)
    return 1;
  else
    return 0;
}

int main(void)
{
  packet p;

  printf("Enter Binary : ");
  //strcpy(p.data, "1011000011110101");
  scanf("%s",p.data);

  p.cksum=checksum(p.data);

  int pos = 0;
  printf("\n\nDATA : %s",p.data);
  printf("\nEnter Position to introduce Error : \n");
  printf("(0 for no Error) : ");
  scanf("%d",&pos);
  printf("\nOLD DATA : %s",p.data);

  if(pos!=0 && pos <17)
  {
    pos = pos - 1;
    if(p.data[pos]=='1')
      p.data[pos]='0';
    else
      p.data[pos]='1';
  }

  printf("\nNEW DATA : %s",p.data);

  if(valid_checksum(p.data,p.cksum))
  {
    printf("\nVALID !!!\n");
  }
  else
    printf("\nINVALID !!!\n");

  return 0;
}