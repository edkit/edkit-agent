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
#include "MemAllocProbe.h"
#include "MemCallocProbe.h"
#include "MemReallocProbe.h"
#include "MemFreeProbe.h"
#include "Controllers.h"

extern "C" {
void *malloc (size_t i_Size) throw();
void *calloc(size_t i_MembCount, size_t i_Size) throw();
void *realloc(void *Ptr, size_t i_Size) throw();
void free (void *Ptr) throw();

void *_Znwj(size_t i_Size);
void *_ZnwjRKSt9nothrow_t(size_t i_Size)  throw();
void _ZdlPvRKSt9nothrow_t (void *Ptr) throw();
void _ZdlPv (void *Ptr);
}


void *malloc (size_t i_Size) throw()
{
   static   bool           InitDone = false;
   static   MemAllocProbe  Probe;
            void           *Data = NULL;

   Probe.InitCheck();

   /* Init all controllers after first alloc is done. set InitState before doing
    * so to avoid infinite recursive calls */
   if(InitDone == false)
   {
      Data = Probe.Alloc(i_Size, __builtin_return_address(0));
      InitDone = true;
      ControllersInit();
   }
   else
      Data = Probe.Alloc(i_Size, __builtin_return_address(0));

   return(Data);
}


void *calloc(size_t i_MembCount, size_t i_Size) throw()
{
   static   MemCallocProbe Probe;
            void           *Data = NULL;

   Probe.InitCheck();
   Data = Probe.Calloc(i_MembCount, i_Size, __builtin_return_address(0));
   return(Data);
}


void *realloc(void *Ptr, size_t i_Size) throw()
{
   static   MemReallocProbe   Probe;
            void              *Data = NULL;

   Probe.InitCheck();
   Data = Probe.Realloc(Ptr, i_Size, __builtin_return_address(0));
   return(Data);
}


void free (void *Ptr) throw()
{
   static   MemFreeProbe  Probe;

   Probe.InitCheck();
   Probe.Free(Ptr);
}


/* new */
void *_Znwj(size_t i_Size)
{
   static   MemAllocProbe  Probe;
   void        *Data = NULL;

   Probe.InitCheck("_Znwj");
   Data = Probe.Alloc(i_Size, __builtin_return_address(0));
   return(Data);
}


/* new (std::nothrow) */
void *_ZnwjRKSt9nothrow_t(size_t i_Size)  throw()
{
   static   MemAllocProbe  Probe;
   void        *Data = NULL;

   Probe.InitCheck("_ZnwjRKSt9nothrow_t");
   Data = Probe.Alloc(i_Size, __builtin_return_address(0));
   return(Data);
}


/* delete (std::nothrow) */
void _ZdlPvRKSt9nothrow_t (void *Ptr) throw()
{
   static   MemFreeProbe  Probe;

   Probe.InitCheck("_ZdlPvRKSt9nothrow_t ");
   Probe.Free(Ptr);
}


/* delete */
void _ZdlPv (void *Ptr)
{
   static   MemFreeProbe  Probe;

   Probe.InitCheck("_ZdlPv");
   Probe.Free(Ptr);

}

