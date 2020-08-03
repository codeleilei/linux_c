#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>


FILE *fp;
size_t write_data(void *ptr,
                 size_t size,
                 size_t nmemb,      
                 void *stream)
{
   int written = fwrite(ptr,size,nmemb,(FILE *)fp);
   return written;
}


int main(int argc,char *argv[])
{
   CURL *curl;
   curl_global_init(CURL_GLOBAL_ALL);
   curl = curl_easy_init();  
   curl_easy_setopt(curl,CURLOPT_URL,argv[1]);

   if((fp = fopen(argv[2],"w")) == NULL)
   {
      puts("please input right name of file!");
      curl_easy_cleanup(curl);
      perror("open");
      return -1;
   }

   curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_data);

   curl_easy_perfom(curl);
   curl_easy_cleanup(curl);
   return 0;
}
