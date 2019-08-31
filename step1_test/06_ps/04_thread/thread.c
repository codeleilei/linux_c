#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void *thread_function(void *arg);
char message[]="THREAD_TEST";

int main()
{
   int res;
   pthread_t a_thread;
   void *thread_t;
   
   res=pthread_create(&a_thread,NULL,thread_function,(void*)message);
   if(res!=0)
   {
      perror("ptr_creat");
      exit(-1);
   }

   puts("wait for thread end!");
   res=pthread_join(a_thread,&thread_t);
   
   if(res!=0)
   {
      perror("wait fail");
      exit(-1);
   }

   printf("thread is over ,result is %s\n",(char *)thread_t);
   
   printf("message is %s\n",message);
   exit(0);
}

void *thread_function(void *arg)
{
   printf("thread is runing,param is %s\n",(char *)arg);
   sleep(3);
   strcpy(message,"modifing!!");
   pthread_exit(" thread is ended");
}
