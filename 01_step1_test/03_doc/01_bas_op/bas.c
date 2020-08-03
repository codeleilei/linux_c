#include <stdio.h>
#include <fcntl.h>	//include open() function
#include <sys/types.h>  //inlcude type mode_t
#include <sys/stat.h>   //include symbol of open
#include <sys/unistd.h>  //include close() function

void main()
{
  int f;
  const char *f_path="test";
  mode_t f_attrib;
  f_attrib= S_IRUSR | S_IWUSR |S_IRGRP|S_IWGRP|S_IROTH;
  f=open(f_path,O_RDONLY);
  if(f==-1)
  {
    f=open(f_path,O_RDWR|O_CREAT,f_attrib);
    if(f!=-1)
      puts("creat a new file");
    else
      puts("fail to creat");
  }else
    puts("success to open file");
  close(f);
	
}
