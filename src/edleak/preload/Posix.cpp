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
#include <errno.h>
#include "PosixSymbol.h"
#include "MemAllocProbe.h"
#include "MemAlignProbe.h"
#include "PosixMemAlignProbe.h"
#include "MemCallocProbe.h"
#include "MemReallocProbe.h"
#include "MemFreeProbe.h"
#include "ContextCallStackList.h"

extern "C" {
#include <stdlib.h>
#include <malloc.h>
}


enum PreloadState
{
   STATE_START,
   STATE_STARTING,
   STATE_STARTED
};

/*
* Probes are all statically allocated to avoid dynamic allocation problems
* during bootstrap. Moreover they are accessed via an accessor so that they can
* be effectively initialized in a deterministic way: These accessors are first
* called in Preload_Init to initialize all the probes.
 */
static MemAllocProbe& Preload_GetMallocProbe(void)
{
   static   MemAllocProbe  Probe(PosixSymbol::malloc());

   return(Probe);
}
static MemAllocProbe& Preload_GetNewProbe(void)
{
   static   MemAllocProbe  Probe(PosixSymbol::new_throw());

   return(Probe);
}
static MemAllocProbe& Preload_GetNewNoThrowProbe(void)
{
   static   MemAllocProbe  Probe(PosixSymbol::new_nothrow());

   return(Probe);
}


static MemAlignProbe& Preload_GetMemAlignProbe(void)
{
   static   MemAlignProbe  Probe(PosixSymbol::memalign());

   return(Probe);
}
static PosixMemAlignProbe& Preload_GetPosixMemAlignProbe(void)
{
   static   PosixMemAlignProbe  Probe(PosixSymbol::posix_memalign());

   return(Probe);
}

static MemCallocProbe& Preload_GetCallocProbe(void)
{
   static   MemCallocProbe  Probe(PosixSymbol::calloc());

   return(Probe);
}
static MemReallocProbe& Preload_GetReallocProbe(void)
{
   static   MemReallocProbe  Probe(PosixSymbol::realloc());

   return(Probe);
}


static MemFreeProbe& Preload_GetFreeProbe(void)
{
   static   MemFreeProbe  Probe(PosixSymbol::free());

   return(Probe);
}
static MemFreeProbe& Preload_GetCFreeProbe(void)
{
   static   MemFreeProbe  Probe(PosixSymbol::cfree());

   return(Probe);
}

static MemFreeProbe& Preload_GetDeleteProbe(void)
{
   static   MemFreeProbe  Probe(PosixSymbol::delete_throw());

   return(Probe);
}
static MemFreeProbe& Preload_GetDeleteNoThrowProbe(void)
{
   static   MemFreeProbe  Probe(PosixSymbol::delete_nothrow());

   return(Probe);
}


static enum PreloadState Preload_Init(void)
{
   static enum PreloadState  State = STATE_START;

   if(State == STATE_START)
   {
      State = STATE_STARTING;
      Preload_GetMallocProbe();
      Preload_GetMemAlignProbe();
      Preload_GetPosixMemAlignProbe();
      Preload_GetNewProbe();
      Preload_GetNewNoThrowProbe();
      Preload_GetCallocProbe();
      Preload_GetReallocProbe();
      Preload_GetFreeProbe();
      Preload_GetCFreeProbe();
      Preload_GetDeleteProbe();
      Preload_GetDeleteNoThrowProbe();
      State = STATE_STARTED;
   }

   return State;
}

void *malloc(size_t i_Size)
{
   void  *Data = NULL;

   if(Preload_Init() == STATE_STARTED)
   {
      MemAllocProbe  &Probe = Preload_GetMallocProbe();
      ContextCallStackList *CallStackList = ContextCallStackList::Instantiate();
      if(CallStackList == NULL)
         return(NULL);

      CallStack Callers;
      UnwindCaller(Callers);
      if(CallStackList->HasItem(Callers.Get()[0]) == true)
         Callers.Unwind();

      Data = Probe.Alloc(i_Size, Callers);
   }
   else
   {
      Data = PosixSymbol::malloc()(i_Size);
   }

   return(Data);
}

void *memalign(size_t i_Boundary, size_t i_Size)
{
   void  *Data = NULL;

   if(Preload_Init() == STATE_STARTED)
   {
      MemAlignProbe  &Probe = Preload_GetMemAlignProbe();
      ContextCallStackList *CallStackList = ContextCallStackList::Instantiate();
      if(CallStackList == NULL)
         return(NULL);

      CallStack Callers;
      UnwindCaller(Callers);
      if(CallStackList->HasItem(Callers.Get()[0]) == true)
         Callers.Unwind();

      Data = Probe.MemAlign(i_Boundary, i_Size, Callers);
   }
   else
   {
      Data = PosixSymbol::memalign()(i_Boundary, i_Size);
   }

   return(Data);
}

int posix_memalign(void **Memptr, size_t i_Alignment, size_t i_Size)
{
   int i_Ret = 0;

   if(Preload_Init() == STATE_STARTED)
   {
      PosixMemAlignProbe  &Probe = Preload_GetPosixMemAlignProbe();
      ContextCallStackList *CallStackList = ContextCallStackList::Instantiate();
      if(CallStackList == NULL)
         return EINVAL;

      CallStack Callers;
      UnwindCaller(Callers);
      if(CallStackList->HasItem(Callers.Get()[0]) == true)
         Callers.Unwind();

      i_Ret = Probe.MemAlign(Memptr, i_Alignment, i_Size, Callers);
   }
   else
   {
      i_Ret = PosixSymbol::posix_memalign()(Memptr, i_Alignment, i_Size);
   }

   return(i_Ret);
}


void *calloc(size_t i_MembCount, size_t i_Size)
{
   void  *Data = NULL;

   if(Preload_Init() == STATE_STARTED)
   {
      MemCallocProbe &Probe = Preload_GetCallocProbe();
      ContextCallStackList *CallStackList = ContextCallStackList::Instantiate();
      if(CallStackList == NULL)
         return(NULL);

      CallStack Callers;
      UnwindCaller(Callers);
      if(CallStackList->HasItem(Callers.Get()[0]) == true)
         Callers.Unwind();

      Data = Probe.Calloc(i_MembCount, i_Size, Callers);
   }
   else
   {
      Data = PosixSymbol::calloc()(i_MembCount, i_Size);
   }

   return(Data);
}


void *realloc(void *Ptr, size_t i_Size)
{
   void              *Data = NULL;

   if(Preload_Init() == STATE_STARTED)
   {
      MemReallocProbe   &Probe = Preload_GetReallocProbe();
      ContextCallStackList *CallStackList = ContextCallStackList::Instantiate();
      if(CallStackList == NULL)
         return(NULL);

      CallStack Callers;
      UnwindCaller(Callers);
      if(CallStackList->HasItem(Callers.Get()[0]) == true)
         Callers.Unwind();

      Data = Probe.Realloc(Ptr, i_Size, Callers);
   }
   else
   {
      Data = PosixSymbol::realloc()(Ptr, i_Size);
   }
   return(Data);
}


void free(void *Ptr)
{
   if(Preload_Init() == STATE_STARTED)
   {
      MemFreeProbe   &Probe = Preload_GetFreeProbe();
      Probe.Free(Ptr);
   }
   else
   {
      PosixSymbol::free()(Ptr);
   }
}


void cfree(void *Ptr)
{
   if(Preload_Init() == STATE_STARTED)
   {
      MemFreeProbe   &Probe = Preload_GetCFreeProbe();
      Probe.Free(Ptr);
   }
   else
   {
      PosixSymbol::cfree()(Ptr);
   }
}


void* operator new(std::size_t i_Size)
{
   void  *Data = NULL;

   if(Preload_Init() == STATE_STARTED)
   {
      MemAllocProbe  &Probe = Preload_GetNewProbe();
      ContextCallStackList *CallStackList = ContextCallStackList::Instantiate();
      if(CallStackList == NULL)
         throw std::bad_alloc();

      CallStack Callers;
      UnwindCaller(Callers);
      if(CallStackList->HasItem(Callers.Get()[0]) == true)
         Callers.Unwind();

      Data = Probe.Alloc(i_Size, Callers);
   }
   else
   {
      Data = PosixSymbol::new_throw()(i_Size);
   }

   return(Data);
}


void* operator new(std::size_t i_Size, const std::nothrow_t&) throw()
{
   void  *Data = NULL;

   if(Preload_Init() == STATE_STARTED)
   {
      MemAllocProbe  &Probe = Preload_GetNewNoThrowProbe();
      ContextCallStackList *CallStackList = ContextCallStackList::Instantiate();
      if(CallStackList == NULL)
         return(NULL);

      CallStack Callers;
      UnwindCaller(Callers);
      if(CallStackList->HasItem(Callers.Get()[0]) == true)
         Callers.Unwind();

      Data = Probe.Alloc(i_Size, Callers);
   }
   else
   {
      Data = PosixSymbol::new_nothrow()(i_Size);
   }

   return(Data);
}


void operator delete(void* Ptr, const std::nothrow_t&)
{
   if(Preload_Init() == STATE_STARTED)
   {
      MemFreeProbe  &Probe = Preload_GetDeleteNoThrowProbe();
      Probe.Free(Ptr);
   }
   else
   {
      PosixSymbol::delete_nothrow()(Ptr);
   }
}


void operator delete(void* Ptr)
{
   if(Preload_Init() == STATE_STARTED)
   {
      MemFreeProbe  &Probe = Preload_GetDeleteProbe();
      Probe.Free(Ptr);
   }
   else
   {
      PosixSymbol::delete_throw()(Ptr);
   }
}
