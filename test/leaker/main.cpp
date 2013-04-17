/*
*****************************************************************************
*                      ___       _   _    _ _
*                     / _ \ __ _| |_| |__(_) |_ ___
*                    | (_) / _` | / / '_ \ |  _(_-<
*                     \___/\__,_|_\_\_.__/_|\__/__/
*                          Copyright (c) 2013
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*****************************************************************************/
/**
* @author   R. Picard
* @date     2013/02/31
*
*****************************************************************************/
#include <unistd.h>
#include <stdlib.h>

#define MAX_DEPTH 8
static void *Trash = NULL;
static void *TrashNew = NULL;

#include <stdio.h>
void Leaker(unsigned int Depth)
{
   static unsigned int Counter = 0;

   Counter++;
   if(Depth == 0)
   {
      static unsigned int Run = 0;
      Run++;
      printf("%d-", Run % (MAX_DEPTH-1));
      switch(Run % (MAX_DEPTH-1))
      {
         case 0:
            Trash = malloc(10);
            break;
         case 1:
            Trash = calloc(1, 100);
            break;
         case 2:
            Trash = realloc(Trash, 1000);
            break;
         case 3:
            Trash = malloc(20);
            break;
         case 4:
            Trash = calloc(1, 200);
            break;
         case 5:
            Trash = realloc(Trash, 2000);
            break;
         case 6:
            TrashNew = new int;
            break;
         case 7:
            TrashNew = new int[50];
            break;
      }
      return;
   }

   printf("%d\n", Counter % MAX_DEPTH);
   switch(Counter % MAX_DEPTH)
   {
      case 0:
         Leaker(Depth-1);
         break;
      case 1:
         Leaker(Depth-1);
         break;
      case 2:
         Leaker(Depth-1);
         break;
      case 3:
         Leaker(Depth-1);
         break;
      case 4:
         Leaker(Depth-1);
         break;
      case 5:
         Leaker(Depth-1);
         break;
      case 6:
         Leaker(Depth-1);
         break;
      case 7:
         Leaker(Depth-1);
         break;
   }
}

int main(int argc, char *argv[])
{
   while(1)
   {
      Leaker(MAX_DEPTH);
      sleep(1);
   }
   return(0);
}
