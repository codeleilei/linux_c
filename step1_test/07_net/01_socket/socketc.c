#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
  int sockfd;
  int len;
  struct sockaddr_in address;
  int result;
  char ch = 'A';
  sockfd = socket(AF_INET,SOCK_STREAM,0);
  
  address.sin_family =AF_INET;
  address.sin_addr.s_addr = inet_addr("127.0.0.1");
  address.sin_port = 9734;
  
  len = sizeof(address);

  result = connect(sockfd,(struct sockaddr *)&address,len);
  
  if(result == -1)
  {
    perror("connect:");
    return 1;
  }

  write(sockfd,&ch,1);
  read(sockfd,&ch,1);
  printf("reciver data from server:%c\n",ch);
  close(sockfd);
  return 0;
}


