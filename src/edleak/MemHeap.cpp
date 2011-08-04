/*
*****************************************************************************
*                      ___       _   _    _ _
*                     / _ \ __ _| |_| |__(_) |_ ___
*                    | (_) / _` | / / '_ \ |  _(_-<
*                     \___/\__,_|_\_\_.__/_|\__/__/      
*                          Copyright (c) 2011
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
* @date     2011/05/01
* 
*****************************************************************************/
#include <new>
#include "MemHeap.h"


/**
* @date     2011/05/01
* 
*  Constructor.
******************************************************************************/
MemHeap::MemHeap(void):
   Locker(NULL), RCounter(0),
   EntryList()
{
   /* The mutex should be a normal data member of the class but mipsel-uclibc
    * does not initialize correctly recursive mutext with settype call. Hence it
    * is statically declared and initialized. */

   static pthread_mutex_t  HeapLocker = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
 
   Locker = &HeapLocker;
   /*
   pthread_mutexattr_t s_Attr;

   pthread_mutexattr_init(&s_Attr);
   pthread_mutexattr_settype(&s_Attr, PTHREAD_MUTEX_RECURSIVE);
   pthread_mutex_init(&Locker, &s_Attr);
   pthread_mutexattr_destroy(&s_Attr);
   */
   return;
}


/**
* @date     2011/05/01
* 
*  Destructor.
*
******************************************************************************/
MemHeap::~MemHeap(void)
{
   //pthread_mutex_destroy(Locker);
   return;
}


/**
* @date     2011/08/03
* 
*  Instantiation of the heap singleton. 
*  The object is static so that it is initialized during the first call.
*  Putting as a global static cannot ensure that it will be initialized before
*  being used if called very early.
*  Moreover we cannot allocate memory here since this will be first called
*  during the instantiation of the first used memory probe (allocating
*  memory here would leak to infinite recursive calls). So a placement new is
*  used to store the Heap space.
*  Note that we cannot directly declare Heap as a static object since some g++
*  implementations add allocations calls in the static objects init code (in
*  __cxa_atexit).
*
* @return Pointer to the MemHeap singleton.
*
******************************************************************************/
MemHeap* MemHeap::Instantiate(void)
{
#define HEAP_DATA_SIZE  512
   static char    HeapData[HEAP_DATA_SIZE];
   static MemHeap *Heap = NULL;

   if(Heap == NULL)
   {
      if(HEAP_DATA_SIZE >= sizeof(MemHeap))
         Heap = new(HeapData) MemHeap();
   }
   return(Heap);
}


/**
* @date     2011/05/01
* 
*  Recursive lock on the heap.
*
******************************************************************************/
int32_t MemHeap::Lock(void)
{
   pthread_mutex_lock(Locker);
   RCounter++;
   if(RCounter == 1)
      return(0);
   else
      return(MEMHEAP_STATUS_RECURSIVE);
}


/**
* @date     2011/05/01
* 
*  Unlocks the heap.
*
******************************************************************************/
void MemHeap::Unlock(void)
{
   RCounter--;
   pthread_mutex_unlock(Locker);
}

