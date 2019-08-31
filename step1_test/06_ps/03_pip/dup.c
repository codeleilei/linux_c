#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
   int fd;
   if((fd= open("output",O_CREAT|O_RDWR,774))== -1)
   {
     perror("open");
     return -1;
   }
   close(1);
   dup(fd);
   close(fd);
   puts("this line will input to file!\n");
   return 0;
}
