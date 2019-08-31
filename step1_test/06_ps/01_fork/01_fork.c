#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  char buf[100]={0};
  int fd,status;
  pid_t cht_pid;

  if((fd=open("fork_test",O_RDWR|O_CREAT,774))==-1)
  {
    perror("open");
    return -1;
  }
  strcpy(buf,"this is father!");

  if((cht_pid=fork())==0)
  {
    strcpy(buf,"this is child!");
    puts("child procedurem is executing!");
    printf("child's pid is %d\n",getpid());
    printf("father's pid is %d\n",getppid());
    write(fd,buf,strlen(buf));
    close(fd);
    exit(0);
  }
  else{
   puts("father procedure is excuting!");
   printf("father's pid is%d\n",getpid());
   printf("child pid is %d\n",cht_pid);
   write(fd,buf,strlen(buf));
   close(fd);
  }
  wait(&status);
  return 1;
}
