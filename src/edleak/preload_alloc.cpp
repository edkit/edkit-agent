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
#include "MemAlignProbe.h"
#include "MemCallocProbe.h"
#include "MemReallocProbe.h"
#include "MemFreeProbe.h"
#include "controller/Controllers.h"

extern "C" {
void *malloc (size_t i_Size) throw();
void *memalign(size_t i_Boundary, size_t i_Size) throw();
void *calloc(size_t i_MembCount, size_t i_Size) throw();
void *realloc(void *Ptr, size_t i_Size) throw();
void free (void *Ptr) throw();

void *CXX_SYM_NEW(size_t i_Size);
void *CXX_SYM_NEW_NOTHROW(size_t i_Size)  throw();
void CXX_SYM_DELETE_NOTHROW(void *Ptr) throw();
void CXX_SYM_DELETE(void *Ptr);
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
   static   MemAllocProbe  Probe;

   return(Probe);
}
static MemAllocProbe& Preload_GetNewProbe(void)
{
   static   MemAllocProbe  Probe;

   return(Probe);
}
static MemAllocProbe& Preload_GetNewNoThrowProbe(void)
{
   static   MemAllocProbe  Probe;

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
      MemAllocProbe     &MallocProbe         = Preload_GetMallocProbe();
      MemAlignProbe     &MemAlignProbe       = Preload_GetMemAlignProbe();
      MemAllocProbe     &NewProbe            = Preload_GetNewProbe();
      MemAllocProbe     &NewNoThrowProbe     = Preload_GetNewNoThrowProbe();
      MemCallocProbe    &CallocProbe         = Preload_GetCallocProbe();
      MemReallocProbe   &ReallocProbe        = Preload_GetReallocProbe();
      MemFreeProbe      &FreeProbe           = Preload_GetFreeProbe();
      MemFreeProbe      &DeleteProbe         = Preload_GetDeleteProbe();
      MemFreeProbe      &DeleteNoThrowProbe  = Preload_GetDeleteNoThrowProbe();

      MallocProbe.InitCheck();
      MemAlignProbe.InitCheck();
      NewProbe.InitCheck(M2STR(CXX_SYM_NEW));
      NewNoThrowProbe.InitCheck(M2STR(CXX_SYM_NEW_NOTHROW));
      CallocProbe.InitCheck();
      ReallocProbe.InitCheck();
      FreeProbe.InitCheck();
      DeleteProbe.InitCheck(M2STR(CXX_SYM_DELETE));
      DeleteNoThrowProbe.InitCheck(M2STR(CXX_SYM_DELETE_NOTHROW));
      State = STATE_STARTED;
   }

   return State;
}

void *malloc (size_t i_Size) throw()
{
   void  *Data = NULL;

   if(Preload_Init() == STATE_STARTED)
   {
      MemAllocProbe  &Probe = Preload_GetMallocProbe();
      CallStack Callers;
      UnwindCaller(Callers);
      Data = Probe.Alloc(i_Size, Callers);
   }
   else
   {
      Data = MemAllocProbe::PassThrough(i_Size);
   }

   return(Data);
}

void *memalign(size_t i_Boundary, size_t i_Size) throw()
{
   void  *Data = NULL;

   if(Preload_Init() == STATE_STARTED)
   {
      MemAlignProbe  &Probe = Preload_GetMemAlignProbe();
      CallStack Callers;
      UnwindCaller(Callers);
      Data = Probe.MemAlign(i_Boundary, i_Size, Callers);
   }
   else
   {
      Data = MemAlignProbe::PassThrough(i_Boundary, i_Size);
   }

   return(Data);
}



void *calloc(size_t i_MembCount, size_t i_Size) throw()
{
   void  *Data = NULL;

   if(Preload_Init() == STATE_STARTED)
   {
      MemCallocProbe &Probe = Preload_GetCallocProbe();
      CallStack Callers;
      UnwindCaller(Callers);
      Data = Probe.Calloc(i_MembCount, i_Size, Callers);
   }
   else
   {
      Data = MemCallocProbe::PassThrough(i_MembCount, i_Size);
   }

   return(Data);
}


void *realloc(void *Ptr, size_t i_Size) throw()
{
   void              *Data = NULL;

   if(Preload_Init() == STATE_STARTED)
   {
      MemReallocProbe   &Probe = Preload_GetReallocProbe();
      CallStack Callers;
      UnwindCaller(Callers);
      Data = Probe.Realloc(Ptr, i_Size, Callers);
   }
   else
   {
      Data = MemReallocProbe::PassThrough(Ptr, i_Size);
   }
   return(Data);
}


void free (void *Ptr) throw()
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


/* new */
void *CXX_SYM_NEW(size_t i_Size)
{
   void  *Data = NULL;

   if(Preload_Init() == STATE_STARTED)
   {
      MemAllocProbe  &Probe = Preload_GetNewProbe();
      CallStack Callers;
      UnwindCaller(Callers);
      Data = Probe.Alloc(i_Size, Callers);
   }
   else
   {
      Data = MemAllocProbe::PassThrough(i_Size, M2STR(CXX_SYM_NEW));
   }

   return(Data);
}


/* new (std::nothrow) */
void *CXX_SYM_NEW_NOTHROW(size_t i_Size)  throw()
{
   void  *Data = NULL;

   if(Preload_Init() == STATE_STARTED)
   {
      MemAllocProbe  &Probe = Preload_GetNewNoThrowProbe();
      CallStack Callers;
      UnwindCaller(Callers);
      Data = Probe.Alloc(i_Size, Callers);
   }
   else
   {
      Data = MemAllocProbe::PassThrough(i_Size, M2STR(CXX_SYM_NEW_NOTHROW));
   }

   return(Data);
}


/* delete (std::nothrow) */
void CXX_SYM_DELETE_NOTHROW(void *Ptr) throw()
{
   if(Preload_Init() == STATE_STARTED)
   {
      MemFreeProbe  &Probe = Preload_GetDeleteNoThrowProbe();
      Probe.Free(Ptr);
   }
   else
   {
      MemFreeProbe::PassThrough(Ptr, M2STR(CXX_SYM_DELETE_NOTHROW));
   }
}


/* delete */
void CXX_SYM_DELETE(void *Ptr)
{
   if(Preload_Init() == STATE_STARTED)
   {
      MemFreeProbe  &Probe = Preload_GetDeleteProbe();
      Probe.Free(Ptr);
   }
   else
   {
      MemFreeProbe::PassThrough(Ptr, M2STR(CXX_SYM_DELETE));
   }
}

