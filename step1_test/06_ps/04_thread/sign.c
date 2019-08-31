#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define WORK_SIZE 100

void * thread_function(void *arg);
sem_t bin_sem;
char work_area[WORK_SIZE];

int main()
{
  int res;
  pthread_t a_thread;
  void *thread_re;

  res=sem_init(&bin_sem,0,0);
  if(res!=0)
  {
     perror("sem");
     exit(-1);
  }

  res= pthread_create(&a_thread,NULL,thread_function,(void*)work_area);
  if(res!=0)
  {
     perror("fail to create thread");
     exit(-1);
  }
  
  printf("please input your message:(quit by end)\n");
  while(strncmp("end",work_area,3)!=0)
  {
     fgets(work_area,WORK_SIZE,stdin);
     sem_post(&bin_sem);    
  }
  
  printf("\nwait for son thread ending!!\n");
  res= pthread_join(a_thread,&thread_re);
  if(res!=0)
  {
     perror("fail to wait for thread");
     exit(-1);
  }
  
  puts("thread done!");
  sem_destroy(&bin_sem);
  exit(0);
}

void *thread_function(void *arg)
{
   sem_wait(&bin_sem);
   while(strncmp("end",work_area,3)!=0)
   {
      printf("have receive %ld char\n",strlen(work_area)-1);
      sem_wait(&bin_sem); 
   }
   pthread_exit(NULL);
}
