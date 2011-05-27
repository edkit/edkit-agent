
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
   void *p_Data;

   p_Data = calloc(1, 10);
   if(p_Data == NULL)
   {
      perror("calloc returns NULL\n");
      return(-1);
   }

   printf("calloced 1x10 bytes\n");
   free(p_Data);
   printf("fred data\n");

   p_Data = calloc(8, 10);
   if(p_Data == NULL)
   {
      perror("calloc returns NULL\n");
      return(-1);
   }

   printf("calloced 8x10 bytes\n");
   free(p_Data);
   printf("fred data\n");

   p_Data = calloc(1, 63);
   if(p_Data == NULL)
   {
      perror("calloc returns NULL\n");
      return(-1);
   }

   printf("calloced 1x63 bytes\n");
   free(p_Data);
   printf("fred data\n");

   p_Data = calloc(5, 63);
   if(p_Data == NULL)
   {
      perror("calloc returns NULL\n");
      return(-1);
   }

   printf("calloced 5x63 bytes\n");
   free(p_Data);
   printf("fred data\n");

   return(0);
}
