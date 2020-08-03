#include <stdio.h>
#include <string.h>
#define LENGTH 50

void main()
{
  char a[LENGTH];
  char *p="ABCDEF";
  strcpy(a,p);
  puts(a);		//abcdef
  strncpy(a,p,3);
  puts(a);		//abcdef
  strcat(a,p);
  puts(a);		//abcdefabcdef
  strncat(a,p,3);
  puts(a);		//abcdefabcdefabc !!!

}
