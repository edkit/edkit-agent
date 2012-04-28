
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
   void *p_Data;

   p_Data = realloc(NULL, 10);
   if(p_Data == NULL)
   {
      perror("realloc returns NULL\n");
      return(-1);
   }

   printf("realloced 10 bytes\n");
   free(p_Data);
   printf("fred data\n");

   p_Data = malloc(15);
   if(p_Data == NULL)
   {
      perror("realloc returns NULL\n");
      return(-1);
   }

   printf("malloced 15 bytes\n");
   free(p_Data);
   printf("fred data\n");

   return(0);
}
