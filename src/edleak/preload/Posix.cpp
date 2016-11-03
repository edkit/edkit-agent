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
#include "PosixSymbol.h"
#include "MemAllocProbe.h"
#include "MemAlignProbe.h"
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
   static   MemAlignProbe  Probe;

   return(Probe);
}
static MemCallocProbe& Preload_GetCallocProbe(void)
{
   static   MemCallocProbe  Probe;

   return(Probe);
}
static MemReallocProbe& Preload_GetReallocProbe(void)
{
   static   MemReallocProbe  Probe;

   return(Probe);
}


static MemFreeProbe& Preload_GetFreeProbe(void)
{
   static   MemFreeProbe  Probe;

   return(Probe);
}
static MemFreeProbe& Preload_GetCFreeProbe(void)
{
   static   MemFreeProbe  Probe;

   return(Probe);
}

static MemFreeProbe& Preload_GetDeleteProbe(void)
{
   static   MemFreeProbe  Probe;

   return(Probe);
}
static MemFreeProbe& Preload_GetDeleteNoThrowProbe(void)
{
   static   MemFreeProbe  Probe;

   return(Probe);
}


static enum PreloadState Preload_Init(void)
{
   static enum PreloadState  State = STATE_START;

   if(State == STATE_START)
   {
      State = STATE_STARTING;
      Preload_GetMallocProbe();
      MemAlignProbe     &MemAlignProbe       = Preload_GetMemAlignProbe();
      Preload_GetNewProbe();
      Preload_GetNewNoThrowProbe();
      MemCallocProbe    &CallocProbe         = Preload_GetCallocProbe();
      MemReallocProbe   &ReallocProbe        = Preload_GetReallocProbe();
      MemFreeProbe      &FreeProbe           = Preload_GetFreeProbe();
      MemFreeProbe      &CFreeProbe          = Preload_GetCFreeProbe();
      MemFreeProbe      &DeleteProbe         = Preload_GetDeleteProbe();
      MemFreeProbe      &DeleteNoThrowProbe  = Preload_GetDeleteNoThrowProbe();

      MemAlignProbe.InitCheck();
      CallocProbe.InitCheck();
      ReallocProbe.InitCheck();
      FreeProbe.InitCheck();
      CFreeProbe.InitCheck("cfree");
      DeleteProbe.InitCheck(CXX_SYM_DELETE);
      DeleteNoThrowProbe.InitCheck(CXX_SYM_DELETE_NOTHROW);
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
      Data = MemAlignProbe::PassThrough(i_Boundary, i_Size);
   }

   return(Data);
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
      Data = MemCallocProbe::PassThrough(i_MembCount, i_Size);
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
      Data = MemReallocProbe::PassThrough(Ptr, i_Size);
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
      MemFreeProbe::PassThrough(Ptr);
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
      MemFreeProbe::PassThrough(Ptr);
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
      MemFreeProbe::PassThrough(Ptr, CXX_SYM_DELETE_NOTHROW);
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
      MemFreeProbe::PassThrough(Ptr, CXX_SYM_DELETE);
   }
}
