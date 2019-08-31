#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>    //
#include <sys/types.h>   //mode_t 
#include <unistd.h>
#define LENGTH 1000

int main()
{
  char c[LENGTH];
  int f,i,j;
  j=0;
  f=open("/usr/include/gnu-versions.h",O_RDONLY,LENGTH);
  if(f!=-1)
  {
    i=read(f,c,LENGTH);
    if(i>0)
    {
      for(;i>0;i--)
	putchar(c[j++]);
    }else
    perror("read:");
  }else
    perror("open:");
  close(f);
  return 0;
}

