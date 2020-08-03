#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define LENGTH 1000 

int main()
{
  int f,j=0;
  char c[LENGTH];
  f=open("test",O_RDWR|O_CREAT,774);
  if(f==-1)
  {
    perror("open and creat");
    return -1;
  }

  if(write(f,"123456",strlen("123456"))!=strlen("123456"))     //123456
    perror("write");

  lseek(f,2,SEEK_SET);
  if(write(f,"abc",strlen("abc"))!=strlen("abc"))              //12abc6
    perror("write_SET");

/****test  read_func**/
  lseek(f,0,SEEK_SET);
  if(read(f,c,6)!=6)
    perror("read");
  for(int i=0;i<7;i++)
	putchar(c[j++]);

 
  lseek(f,-2,SEEK_END);
  if(write(f,"789",strlen("789"))!=strlen("789"))              //12ab789
    perror("write_END");
  
  lseek(f,2,SEEK_CUR);
  if(write(f,"cl",strlen("clnb"))!=strlen("clnb"))              //12ab789clnb
    perror("write_END");
   
  return 1;
}
