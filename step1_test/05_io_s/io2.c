#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#define TE_NUM 3

struct buffy{
  char name[500];
  unsigned long int tel;
  char address[200];
};

struct buff{
  unsigned int length;
 fpos_t pos;
};

struct buffy budd;
struct buff budy[TE_NUM];

int main()
{
  if(creat("body2",774)==-1)
  {
    perror("creat");
    return -1;
  }
  
  FILE *fp;
  int i;

  fp=fopen("body2","rw+");
  for(i=0;i<TE_NUM;i++)
  {
    fgetpos(fp,&(budy[i].pos));
    budy[i].length=fprintf(fp,"this %d to putout the data",i);    
  }
  fflush(fp);
  printf("totall length equal %ld\n",ftell(fp));

  rewind(fp);     //reset point
  for(i=0;i<TE_NUM;i++)
  {
    fsetpos(fp,&(budy[i].pos));
    char *buf=malloc(budy[i].length+1);
    *(buf+budy[i].length+1)='\0';
    fread(buf,budy[i].length,1,fp);
    puts(buf);
    free(buf);
  }
 
  fclose(fp);
  return 1;
}
