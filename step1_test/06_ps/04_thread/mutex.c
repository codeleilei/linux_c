#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define WORK_SIZE 1000

void *thread_fun(void* arg);

pthread_mutex_t work_mutex;
char work_buf[WORK_SIZE];
int time_to_exit = 1;

int main(int argc,char *argv[])
{
  int res;
  pthread_t a_thread;
  void *thread_re;
  
  res=pthread_mutex_init(&work_mutex,NULL);
  if(res!=0)
  {
    perror("creat mutex fail");
    exit(-1);
  }

  res=pthread_create(&a_thread,NULL,thread_fun,NULL);
  if(res!=0)
  {
    perror("creat pthread fail");
    exit(-1);
  }

  pthread_mutex_lock(&work_mutex);
  printf("please input message,fnished by 'end'\n");
  while(time_to_exit)
  {
    fgets(work_buf,WORK_SIZE,stdin);
    pthread_mutex_unlock(&work_mutex);
    while(1)
    {
       pthread_mutex_lock(&work_mutex);
       if(work_buf[0] !='\0')
       {
	   pthread_mutex_unlock(&work_mutex);
 	   sleep(1);
       } 
      else
	break;
    }
  }
  pthread_mutex_unlock(&work_mutex);

  printf("wait for thread over!\n");
  res=pthread_join(a_thread,&thread_re);
  if(res!=0)
  {
    perror("thread over fail");
    exit(-1);
  }
  
  printf("thread is over!\n");
  pthread_mutex_destroy(&work_mutex);
  exit(0); 
}

void *thread_fun(void* arg)
{
  sleep(1);
  pthread_mutex_lock(&work_mutex);
  while(strncmp("end",work_buf,3) != 0)
  {
    printf("have receive %ld char\n",strlen(work_buf)-1);
    work_buf[0]='\0';
    pthread_mutex_unlock(&work_mutex);
    sleep(1);
    pthread_mutex_lock(&work_mutex);
    while(work_buf[0]== '\0')
    {
       pthread_mutex_unlock(&work_mutex);
       sleep(1);
       pthread_mutex_lock(&work_mutex);
    }   
  } 
  time_to_exit=0;
  work_buf[0]='\0';
  pthread_mutex_unlock(&work_mutex);
  pthread_exit(0);
}
