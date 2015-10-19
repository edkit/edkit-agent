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
* @date     2013/07/03
*
*****************************************************************************/
#include <new>
#include "SliceAllocProbe.h"
#include "ExeContext.h"
#include "HeapEntry.h"

namespace GLib {

/**
* @date     2013/07/03
*
*  Constructor.
*
******************************************************************************/
SliceAllocProbe::SliceAllocProbe(void):
   MemProbe(),
   AllocFunc(NULL)
{
   return;
}


/**
* @date     2013/07/03
*
*  Destructor.
*
******************************************************************************/
SliceAllocProbe::~SliceAllocProbe()
{
   return;
}


/**
* @date     2013/07/03
*
*  Probe initialization.
*
* @param sz_AllocFunc (in): Name of the allocation function. "mg_slice_alloc"
                              is used if NULL is provided here.
*
******************************************************************************/
void SliceAllocProbe::InitCheck(const char *sz_AllocFunc)
{
   if(AllocFunc == NULL)
   {
      if(sz_AllocFunc != NULL)
         AllocFunc = (slice_alloc_t) rtsym_resolve(sz_AllocFunc);
      else
         AllocFunc = (slice_alloc_t) rtsym_resolve("g_slice_alloc");
   }
   return;
}


/**
* @date     2013/07/03
*
*  Probe Passthrough : The original function is called direclty.
*
******************************************************************************/
gpointer SliceAllocProbe::PassThrough(gsize Size, const char *sz_AllocFunc)
{
   uint8_t *Data = NULL;
   malloc_t AllocFunc;

   if(sz_AllocFunc != NULL)
      AllocFunc = (malloc_t) rtsym_resolve(sz_AllocFunc);
   else
      AllocFunc = (malloc_t) rtsym_resolve("g_slice_alloc");

   if(AllocFunc != NULL)
   {
      Data = (uint8_t*)AllocFunc(Size);
   }
   return(Data);
}

/**
* @date     2013/07/03
*
*  Allocates some data by using the internal allocer.
*
* @param Size (in): Size to allocate.
* @param Callers (in): Callers address.
*
******************************************************************************/
gpointer SliceAllocProbe::Alloc(gsize Size, const CallStack& Callers)
{
   uint8_t *Data = NULL;
   uint32_t Padding = MemProbe::GetAlignmentPadding(ALLOC_ALIGNMENT, sizeof(HeapEntry));

   if(AllocFunc != NULL)
   {
      if(GetHeap()->Lock() == 0)
      {
         Data = (uint8_t*)AllocFunc(Size + sizeof(HeapEntry) + Padding);
         if(Data != NULL)
         {
            ExeContext *p_Context = ExeContext::Get(Callers);
            HeapEntry  *Entry = new (Data + Padding) HeapEntry(Size, p_Context);

            Entry->Start = Data;
            GetHeap()->GetEntryList()->AppendItem(Entry);
            if(p_Context != NULL)
               p_Context->UpdateMemory(Size);

            Data = Data + sizeof(HeapEntry) + Padding;
         }
      }
      else
      {
         // recursive lock, do not try to monitor it. This can occur in
         // different cases:
         // - some implementations base calloc/realloc on malloc.
         // - New ExeContext objects can be created in the context of an
         // allocation.
         Data = (uint8_t*)AllocFunc(Size);
      }

      GetHeap()->Unlock();

   }
   return(Data);
}

} // GLib
