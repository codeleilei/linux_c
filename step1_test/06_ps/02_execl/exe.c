#include <unistd.h>
#include <stdio.h>

extern char **environ;

int main(int argc,char* argv[])
{
   puts("this msg may can't out");
   execve("exec",argv,environ);
   puts("can't see it at normal ");
   return 1; 
}
