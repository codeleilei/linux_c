#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>


int main(int argc, char *argv[])
{
  char *host;
  int socketfd;
  int len,result;
  struct sockaddr_in address;
  struct hostent *hostinfo;
  struct servent *servinfo;
  char buffer[128];

  if(argc == 1)
     host = "localhost";
  else
     host = argv[1];
  
  hostinfo = gethostbyname(host);
  if(!hostinfo)
  {
    fprintf(stderr,"can't find host: %s\n",host);
    return -1;
  }
  
  servinfo = getservbyname("daytime","tcp");
  if(!servinfo)
  {
    fprintf(stderr,"don't have server of daytime\n");
    return -1;
  }
  
  printf("daytime port is : %d\n",servinfo->s_port);
  
  socketfd = socket(AF_INET,SOCK_STREAM,0);
  address.sin_family = AF_INET;
  address.sin_port = servinfo->s_port;
  address.sin_addr = *(struct in_addr *) *hostinfo->h_addr_list;

  len = sizeof(address);
  result = connect(socketfd,(struct sockaddr *)&address,len);

  if(result == -1)
  {
    perror("connect fail");
    return -1;
  }

  result = read(socketfd,buffer,sizeof(buffer));
  buffer[result]='\0';
  
  printf("read %d char: %s\n",result,buffer);
  close(socketfd);
  return 0; 
}
