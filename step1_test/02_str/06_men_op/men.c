#include <stdio.h>
#include <string.h>

void main()
{
  char s1[20];
  const char *s2 ="linux Kernel";
  char* p;
  memcpy(s1,s2,13);
  puts(s1);
  memmove(s1,s1+6,6);
  puts(s1);
  printf("result of comparing string of s1 and s1+6:%d\n",memcmp(s1,s1+6,6));
  p=memchr(s2,'x',7);
  puts(p);
  puts(memset(s1,'A',6));
}
