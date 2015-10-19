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
* @date     2011/05/03
*
*****************************************************************************/
#include <new>
#include "MemReallocProbe.h"
#include "ExeContext.h"
#include "HeapEntry.h"

/**
* @date     2011/05/03
*
*  Constructor.
*
******************************************************************************/
MemReallocProbe::MemReallocProbe(void):
   MemProbe(),
   AllocFunc(NULL)
{
   return;
}


/**
* @date     2011/05/03
*
*  Destructor.
*
******************************************************************************/
MemReallocProbe::~MemReallocProbe()
{
   return;
}


/**
* @date     2011/05/03
*
*  Probe initialization.
*
******************************************************************************/
void MemReallocProbe::InitCheck(void)
{
   if(AllocFunc == NULL)
      AllocFunc = (realloc_t)rtsym_resolve("realloc");
   return;
}


/**
* @date     2012/01/02
*
*  Probe Passthrough : The original function is called direclty.
*
******************************************************************************/
void* MemReallocProbe::PassThrough(void *Ptr, size_t i_Size)
{
   uint8_t     *Data = NULL;
   realloc_t   AllocFunc;

   AllocFunc = (realloc_t)rtsym_resolve("realloc");
   if(AllocFunc != NULL)
   {
      Data = (uint8_t*)AllocFunc(Ptr, i_Size);
   }
   return(Data);
}

/**
* @date     2011/05/03
*
*  Re-allocates some data.
*
* @param Ptr (in): Pointer to re-allocate.
* @param i_Size (in): Size to allocate.
* @param Callers (in): Callers address.
******************************************************************************/
void* MemReallocProbe::Realloc(void *Ptr, size_t i_Size, const CallStack& Callers)
{
   uint8_t *Data = NULL;
   uint32_t Padding = MemProbe::GetAlignmentPadding(ALLOC_ALIGNMENT, sizeof(HeapEntry));

   if(AllocFunc != NULL)
   {
      if(GetHeap()->Lock() == 0)
      {
         if(Ptr != NULL)
         {
            HeapEntry *Entry = (HeapEntry*)((uint8_t*)Ptr - sizeof(HeapEntry));

            if(Entry->Magic == HEAPENTRY_MAGIC)
            {
               /* remove existing entry from embedded heap */
               GetHeap()->GetEntryList()->DelItem(Entry);
               Entry->Magic = 0;
               if(Entry->Context != NULL)
                  Entry->Context->UpdateMemory(-(Entry->Size));

               if(Entry->Start != NULL) // there is some padding on allocated chunk
                  Data = (uint8_t*)Entry->Start;
               else
                  Data = (uint8_t*)Entry;
            }
         }
         Data = (uint8_t*)AllocFunc(Data, i_Size + sizeof(HeapEntry)+Padding);
         if(Data != NULL)
         {
            ExeContext *p_Context = ExeContext::Get(Callers);
            HeapEntry  *Entry = new (Data+Padding) HeapEntry(i_Size, p_Context);

            Entry->Start = Data;
            GetHeap()->GetEntryList()->AppendItem(Entry);
            if(p_Context != NULL)
               p_Context->UpdateMemory(i_Size);

            Data = Data + sizeof(HeapEntry) + Padding;
         }
      }
      else  // recursive lock, do not try to monitor it
      {
         Data = (uint8_t*)AllocFunc(Ptr, i_Size);
      }

      GetHeap()->Unlock();

   }
   return(Data);
}

