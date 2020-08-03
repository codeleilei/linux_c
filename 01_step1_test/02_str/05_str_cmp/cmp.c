#include <stdio.h>
#include <string.h>

void main()
{
  const char* p1="Happy New Year!";
  const char* p2="Year";
  const char* p3="pye HwNa";
  char* p;
  p=strchr(p1,'a');
  printf("alphbet a first show at :");
  p != NULL? puts(p):puts("no");
 
  p=strrchr(p1,'a');  
  printf("alphbet a last show at :");
  p != NULL? puts(p):puts("no");

  printf("length of not meet str of s2:%ld\n",strcspn(p1,p2));

  printf("length of meet str of s2:%ld\n",strspn(p1,p2));

  p=strpbrk(p1,p2);  
  printf("alphbet p2 last show at :");
  p != NULL? puts(p):puts("no");

  p=strstr(p1,p2);  
  printf("alphbet p2 last show at :");
  p != NULL? puts(p):puts("no");

  
}
