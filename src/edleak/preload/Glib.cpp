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
* @date     2013/08/01
*
*****************************************************************************/
#include "SliceAllocProbe.h"
#include "SliceFreeProbe.h"
#include "ContextCallStackList.h"

extern "C" {
gpointer g_slice_alloc(gsize block_size);
void     g_slice_free1(gsize block_size,
                       gpointer mem_block);
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
static GLib::SliceAllocProbe& Preload_GetSliceAllocProbe(void)
{
   static   GLib::SliceAllocProbe  Probe;

   return(Probe);
}

static GLib::SliceFreeProbe& Preload_GetSliceFreeProbe(void)
{
   static   GLib::SliceFreeProbe  Probe;

   return(Probe);
}

static enum PreloadState Preload_Init(void)
{
   static enum PreloadState  State = STATE_START;

   if(State == STATE_START)
   {
      State = STATE_STARTING;
      GLib::SliceAllocProbe   &SliceAallocProbe    = Preload_GetSliceAllocProbe();
      GLib::SliceFreeProbe    &SliceFreeProbe      = Preload_GetSliceFreeProbe();

      SliceAallocProbe.InitCheck();
      SliceFreeProbe.InitCheck();
      State = STATE_STARTED;
   }

   return State;
}

gpointer g_slice_alloc(gsize i_Size)
{
   void  *Data = NULL;

   if(Preload_Init() == STATE_STARTED)
   {
      GLib::SliceAllocProbe  &Probe = Preload_GetSliceAllocProbe();
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
      Data = GLib::SliceAllocProbe::PassThrough(i_Size);
   }

   return(Data);
}


void     g_slice_free1(gsize i_Size,
                       gpointer Ptr)
{
   if(Preload_Init() == STATE_STARTED)
   {
      GLib::SliceFreeProbe   &Probe = Preload_GetSliceFreeProbe();
      Probe.Free(i_Size, Ptr);
   }
   else
   {
      GLib::SliceFreeProbe::PassThrough(i_Size, Ptr);
   }
}


