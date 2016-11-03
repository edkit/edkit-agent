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
#include "MemCallocProbe.h"
#include "ExeContext.h"
#include "HeapEntry.h"


/**
* @date     2011/05/03
*
*  Constructor.
*
******************************************************************************/
MemCallocProbe::MemCallocProbe(calloc_t Allocator):
   MemProbe(),
   AllocFunc(Allocator)
{
   return;
}


/**
* @date     2011/05/03
*
*  Destructor.
*
******************************************************************************/
MemCallocProbe::~MemCallocProbe()
{
   return;
}


/**
* @date     2011/05/03
*
*  Allocates some data by using the internal allocer.
*
* @param i_MembCount (in): Number of elements to allocate.
* @param i_Size (in): Size of an element.
* @param Callers (in): Callers address.
*
******************************************************************************/
void* MemCallocProbe::Calloc(size_t i_MembCount, size_t i_Size, const CallStack& Callers)
{
   uint8_t *Data = NULL;
   uint32_t Padding = MemProbe::GetAlignmentPadding(ALLOC_ALIGNMENT, sizeof(HeapEntry));

   if(AllocFunc != NULL)
   {
      if(GetHeap()->Lock() == 0)
      {
         /* just try to add an overhead as small as possible */
         if( (i_MembCount > 1) && (i_Size > (sizeof(HeapEntry)+Padding)) )
            Data = (uint8_t*)AllocFunc(i_MembCount+1, i_Size);
         else
            Data = (uint8_t*)AllocFunc(i_MembCount, i_Size+sizeof(HeapEntry)+Padding);

         if(Data != NULL)
         {
            ExeContext *p_Context = ExeContext::Get(Callers);
            HeapEntry  *Entry = new (Data+Padding) HeapEntry(i_Size*i_MembCount, p_Context);

            Entry->Start = Data;
            GetHeap()->GetEntryList()->AppendItem(Entry);
            if(p_Context != NULL)
               p_Context->UpdateMemory((i_Size*i_MembCount));
            Data = Data + sizeof(HeapEntry) + Padding;
         }
      }
      else  // recursive lock, do not try to monitor it
      {
         Data = (uint8_t*)AllocFunc(i_MembCount, i_Size);
      }

      GetHeap()->Unlock();
   }
   return(Data);
}
