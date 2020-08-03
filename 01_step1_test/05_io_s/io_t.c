#include <stdio.h>
#include <fcntl.h>


struct buffy{
  char name[500];
  unsigned long int tel;
  char address[200];
};

struct buffy budd;
int main()
{
  if(creat("body",774)==-1)
  {
    perror("creat");
    return -1;
  }
  
  FILE *fp;
  fp=fopen("body","rw+");
  fprintf(fp,"<name>%s <tel>%ld <address>%s","TOM",15788888888,"CHINA");
  fclose(fp);
  
  fp=fopen("body","rw+");
  fscanf(fp,"<name>%s <tel>%ld <address>%s",budd.name,&budd.tel,budd.address);
  fclose(fp);

  printf("name:%s,tel:%ld,address:%s\n",budd.name,budd.tel,budd.address);
  return 1;
}
