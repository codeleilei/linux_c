#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int f;
  const char *f_path="test";
  mode_t f_parm;
  struct stat *buf=malloc(sizeof(stat));
  f_parm=S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IWOTH|S_IROTH;
  f=creat(f_path,f_parm);
  if(f==-1)
  {
    puts("creat a failure");
    return 1;
  }
  else
    puts("creat a successfull");
  fstat(f,buf);
  if(buf->st_mode & S_IRUSR)
	puts("usrer have root of read");
  if(buf->st_mode & S_IWGRP)
	puts("group have root of write");
  close(f);
  chmod(f_path,0771);
  stat(f_path,buf);
  if(buf->st_mode & S_IRUSR)
	puts("usrer have root of read");
  if(buf->st_mode & S_IWGRP)
	puts("group have root of write");
  free(buf);
  return 0;
}
