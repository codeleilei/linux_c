#include <stdio.h>
#include <ctype.h>
#define LENGTH 50
void main()
{
  char c[LENGTH]="China NO.1";
  puts(c);
  for(int i=0;i<LENGTH;i++)
  {
	if(isalpha(c[i]))
	  if(islower(c[i]))
	     c[i]=toupper(c[i]);
  }
  puts(c);
}
