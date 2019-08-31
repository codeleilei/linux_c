#include <stdio.h>
#include <stdlib.h>
int main()
{
  const char *c="87.92% complate";
  char *p;
  double d;
  d = strtod(c,&p);
  printf("after change double str:%.2f\n",d);
  printf("behind %.2f is %s\n",d,p);
  return 1;
}
