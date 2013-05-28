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
* @date     2011/05/08
*
*****************************************************************************/
#include "Thread.h"


/**
* @date     2011/05/08
*
*  Constructor.
*
******************************************************************************/
Thread::Thread(void):
   ThreadId(0),
   Started(false)
{
   return;
}


/**
* @date     2011/05/08
*
*  Destructor.
*
******************************************************************************/
Thread::~Thread(void)
{
   return;
}


/**
* @date     2011/05/08
*
*  Starts a new thread.
*
*  @return  0 if success.
*  @return  -1 otherwise.
******************************************************************************/
int32_t Thread::Start(void)
{
   if(pthread_create(&ThreadId, NULL, Thread::PThreadLoop, this) != 0)
      return(-1);

   Started = true;
   return(0);
}


/**
* @date     2011/05/08
*
*  Blocks until the thread has terminated. This API is not multi-thread safe, it
*  must be called from only one thread.
*
*  @return  0 if success.
*  @return  -1 otherwise.
******************************************************************************/
int32_t Thread::WaitForEnd(void)
{
   if(Started == false)
      return(0);

   if(pthread_join(ThreadId, NULL) == 0)
   {
      Started = false;
      return(0);
   }
   return(-1);
}


/**
* @date     2011/05/08
*
*  static method for pthread entry point. Just calls the loop method.
*
******************************************************************************/
void* Thread::PThreadLoop(void* p_Thread)
{
   Thread *This = (Thread*)p_Thread;

   if(This != NULL)
      This->Loop();
   return(NULL);
}
