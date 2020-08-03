#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

int main()
{
  int fd[2],cld_pid,len;
  char buf[200];
 
  if(pipe(fd)==-1)
  {
     perror("pipe");
     exit(-1);
  }

  if(cld_pid=fork()==0)
  {
     puts("cld begin!\n");
     close(fd[1]);
     len=read(fd[0],buf,sizeof(buf));
     buf[len]=0;
    
     printf("read form pipe is %s\n",buf);
     puts("cld end!!\n");
     exit(0);
  }
  else
  {
     puts("father begin!!\n");
     close(fd[0]);
     sprintf(buf,"father creat for cld (pid=%d)",cld_pid);

     write(fd[1],buf,sizeof(buf));
     puts("father end!!\n");
     exit(0);
  }
  return 0;
}
