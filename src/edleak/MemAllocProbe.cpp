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
#include "MemAllocProbe.h"
#include "ExeContext.h"
#include "HeapEntry.h"


/**
* @date     2011/05/01
*
*  Constructor.
*
******************************************************************************/
MemAllocProbe::MemAllocProbe(void):
   MemProbe(),
   AllocFunc(NULL)
{
   return;
}


/**
* @date     2011/05/01
*
*  Destructor.
*
******************************************************************************/
MemAllocProbe::~MemAllocProbe()
{
   return;
}


/**
* @date     2011/05/01
*
*  Probe initialization.
*
* @param sz_AllocFunc (in): Name of the allocation function. "malloc" is used if
*                             NULL is provided here.
*
******************************************************************************/
void MemAllocProbe::InitCheck(const char *sz_AllocFunc)
{
   if(AllocFunc == NULL)
   {
      if(sz_AllocFunc != NULL)
         AllocFunc = (malloc_t) rtsym_resolve(sz_AllocFunc);
      else
         AllocFunc = (malloc_t) rtsym_resolve("malloc");
   }
   return;
}


/**
* @date     2011/11/21
*
*  Probe initialization.
*
* @param BaseAllocFunc (in): Pointer to the allocation function. "malloc" is used if
*                             NULL is provided here.
*
******************************************************************************/
void MemAllocProbe::InitCheck(malloc_t BaseAllocFunc)
{
   if(AllocFunc == NULL)
   {
      if(BaseAllocFunc != NULL)
         AllocFunc = BaseAllocFunc;
      else
         AllocFunc = (malloc_t) rtsym_resolve("malloc");
   }
   return;
}


/**
* @date     2012/01/02
*
*  Probe Passthrough : The original function is called direclty.
*
******************************************************************************/
void* MemAllocProbe::PassThrough(size_t i_Size, const char *sz_AllocFunc)
{
   uint8_t *Data = NULL;
   malloc_t AllocFunc;

   if(sz_AllocFunc != NULL)
      AllocFunc = (malloc_t) rtsym_resolve(sz_AllocFunc);
   else
      AllocFunc = (malloc_t) rtsym_resolve("malloc");

   if(AllocFunc != NULL)
   {
      Data = (uint8_t*)AllocFunc(i_Size);
   }
   return(Data);
}

/**
* @date     2011/05/01
*
*  Allocates some data by using the internal allocer.
*
* @param i_Size (in): Size to allocate.
* @param Eip (in): Caller address.
*
******************************************************************************/
void* MemAllocProbe::Alloc(size_t i_Size, void *Eip)
{
   uint8_t *Data = NULL;
   uint32_t Padding = MemProbe::GetAlignmentPadding(ALLOC_ALIGNMENT, sizeof(HeapEntry));

   if(AllocFunc != NULL)
   {
      if(GetHeap()->Lock() == 0)
      {
         Data = (uint8_t*)AllocFunc(i_Size + sizeof(HeapEntry) + Padding);
         if(Data != NULL)
         {
            ExeContext *p_Context = ExeContext::Get(Eip);
            HeapEntry  *Entry = new (Data + Padding) HeapEntry(i_Size, p_Context);

            Entry->Start = Data;
            GetHeap()->GetEntryList()->AppendItem(Entry);
            if(p_Context != NULL)
               p_Context->Memory += i_Size;

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
         Data = (uint8_t*)AllocFunc(i_Size);
      }

      GetHeap()->Unlock();

   }
   return(Data);
}

