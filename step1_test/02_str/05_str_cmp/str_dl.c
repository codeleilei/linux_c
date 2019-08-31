#include <stdio.h>
#include <string.h>

void main()
{
  char s[]="linux is wonderful thing";
  char* p;
  p=strtok(s," ");
  while(p!=NULL)
  {
    puts(p);
    p=strtok(NULL," ");
  }
}
