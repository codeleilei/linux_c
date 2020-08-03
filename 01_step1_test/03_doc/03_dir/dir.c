#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

int scan_dir(char *dir,int dap); //dap is num of space
int main()
{
  printf("scan dir /boot");
  scan_dir("/boot",0);
  puts("end of scan");
  return 0;
}

int scan_dir(char *dir,int dap)
{
  DIR *dp;
  struct dirent *entry;
  struct stat  statbuf;
  if((dp=opendir(dir))==NULL)
  {
    puts("can't open this dir");
    return -1;
  }
  chdir(dir);
  while((entry= readdir(dp))!= NULL)
  {
    lstat(entry->d_name,&statbuf);
    if(S_IFDIR & statbuf.st_mode)
    {
	if(strcmp(".",entry->d_name)==0 || strcmp("..",entry->d_name)==0)
	  continue;
	printf("%*s%s\n",dap,"",entry->d_name);
	scan_dir(entry->d_name,dap+4); 
    }
    else
	printf("%*s%s\n",dap,"",entry->d_name);
  }
  chdir("..");
  closedir(dp);
  return 1;
}
