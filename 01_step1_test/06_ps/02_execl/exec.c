#include <stdio.h>
#include <unistd.h>

extern char **environ;

int main(int argc,char *argv[])
{
  int i;
  puts("putout param:");
  for(i=0;i<=argc;i++)
    printf("param %d is %s\n",i,argv[i]);

  puts("putout env param:");

  for(i=0;environ[i]!=NULL;i++)
    printf("%s\n",environ[i]);
  return 1;
}
