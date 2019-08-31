#include <stdio.h>
#include <stdlib.h>
#define LENGTH 8
void main()
{
  double d=1234.6788;
  char p[LENGTH+2];
  gcvt(d,LENGTH,p);
  puts(p);

  char q[50];
  sprintf(q,"%4.2f%s",d,"translated");
  puts(q);

}
