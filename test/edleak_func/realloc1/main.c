
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
   void *p_Data;

   p_Data = malloc(10);
   if(p_Data == NULL)
   {
      perror("malloc returns NULL\n");
      return(-1);
   }
   p_Data = realloc(p_Data, 15);
   if(p_Data == NULL)
   {
      perror("realloc returns NULL\n");
      return(-1);
   }

   printf("realloced 15 bytes\n");
   free(p_Data);
   printf("fred data\n");
   return(0);
}
