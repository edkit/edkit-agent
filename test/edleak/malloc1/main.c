
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
   int i;
   void *p_Data;

   for(i=0; i<23; i++)
   {
      p_Data = malloc(10);
      sleep(5);
      if(p_Data == NULL)
      {
         perror("malloc returns NULL\n");
         return(-1);
      }
   }

   printf("malloced 10 bytes\n");
   free(p_Data);
   printf("fred 10 bytes\n");
   return(0);
}
