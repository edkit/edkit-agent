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
      MemAllocProbe     &NewProbe            = Preload_GetNewProbe();
      MemAllocProbe     &NewNoThrowProbe     = Preload_GetNewNoThrowProbe();
      MemCallocProbe    &CallocProbe         = Preload_GetCallocProbe();
      MemReallocProbe   &ReallocProbe        = Preload_GetReallocProbe();
      MemFreeProbe      &FreeProbe           = Preload_GetFreeProbe();
      MemFreeProbe      &DeleteProbe         = Preload_GetDeleteProbe();
      MemFreeProbe      &DeleteNoThrowProbe  = Preload_GetDeleteNoThrowProbe();

      MallocProbe.InitCheck();
      NewProbe.InitCheck("_Znwj");
      NewNoThrowProbe.InitCheck("_ZnwjRKSt9nothrow_t");
      CallocProbe.InitCheck();
      ReallocProbe.InitCheck();
      FreeProbe.InitCheck();
      DeleteProbe.InitCheck("_ZdlPv");
      DeleteNoThrowProbe.InitCheck("_ZdlPvRKSt9nothrow_t");
      State = STATE_STARTED;
      ControllersInit();
   }

   return State;
}

void *malloc (size_t i_Size) throw()
{
   void  *Data = NULL;

   if(Preload_Init() == STATE_STARTED)
   {
      MemAllocProbe  &Probe = Preload_GetMallocProbe();
      Data = Probe.Alloc(i_Size, __builtin_return_address(0));
   }
   else
   {
      Data = MemAllocProbe::PassThrough(i_Size);
   }

   return(Data);
}


void *calloc(size_t i_MembCount, size_t i_Size) throw()
{
   void  *Data = NULL;

   if(Preload_Init() == STATE_STARTED)
   {
      MemCallocProbe &Probe = Preload_GetCallocProbe();
      Data = Probe.Calloc(i_MembCount, i_Size, __builtin_return_address(0));
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
      Data = Probe.Realloc(Ptr, i_Size, __builtin_return_address(0));
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
void *_Znwj(size_t i_Size)
{
   void  *Data = NULL;

   if(Preload_Init() == STATE_STARTED)
   {
      MemAllocProbe  &Probe = Preload_GetNewProbe();
      Data = Probe.Alloc(i_Size, __builtin_return_address(0));
   }
   else
   {
      Data = MemAllocProbe::PassThrough(i_Size, "_Znwj");
   }

   return(Data);
}


/* new (std::nothrow) */
void *_ZnwjRKSt9nothrow_t(size_t i_Size)  throw()
{
   void  *Data = NULL;

   if(Preload_Init() == STATE_STARTED)
   {
      MemAllocProbe  &Probe = Preload_GetNewNoThrowProbe();
      Data = Probe.Alloc(i_Size, __builtin_return_address(0));
   }
   else
   {
      Data = MemAllocProbe::PassThrough(i_Size, "_ZnwjRKSt9nothrow_t");
   }

   return(Data);
}


/* delete (std::nothrow) */
void _ZdlPvRKSt9nothrow_t (void *Ptr) throw()
{
   if(Preload_Init() == STATE_STARTED)
   {
      MemFreeProbe  &Probe = Preload_GetDeleteNoThrowProbe();
      Probe.Free(Ptr);
   }
   else
   {
      MemFreeProbe::PassThrough(Ptr, "_ZdlPvRKSt9nothrow_t");
   }
}


/* delete */
void _ZdlPv (void *Ptr)
{
   if(Preload_Init() == STATE_STARTED)
   {
      MemFreeProbe  &Probe = Preload_GetDeleteProbe();
      Probe.Free(Ptr);
   }
   else
   {
      MemFreeProbe::PassThrough(Ptr, "_ZdlPv");
   }
}

