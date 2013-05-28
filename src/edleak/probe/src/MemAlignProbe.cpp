/*
*****************************************************************************
*                      ___       _   _    _ _
*                     / _ \ __ _| |_| |__(_) |_ ___
*                    | (_) / _` | / / '_ \ |  _(_-<
*                     \___/\__,_|_\_\_.__/_|\__/__/
*                          Copyright (c) 2012
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
* @date     2012/01/03
*
*****************************************************************************/
#include <new>
#include "MemAlignProbe.h"
#include "ExeContext.h"
#include "HeapEntry.h"


/**
* @date     2012/01/03
*
*  Constructor.
*
******************************************************************************/
MemAlignProbe::MemAlignProbe(void):
   MemProbe(),
   AlignFunc(NULL)
{
   return;
}


/**
* @date     2012/01/03
*
*  Destructor.
*
******************************************************************************/
MemAlignProbe::~MemAlignProbe()
{
   return;
}


/**
* @date     2012/01/03
*
*  Probe initialization.
*
******************************************************************************/
void MemAlignProbe::InitCheck(void)
{
   if(AlignFunc == NULL)
   {
      AlignFunc = (memalign_t) rtsym_resolve("memalign");
   }
   return;
}


/**
* @date     2012/04/29
*
*  Probe initialization. This method is used for unitest so that a fake align
*  function can be used.
*
******************************************************************************/
void MemAlignProbe::InitCheck(memalign_t AlignHook)
{
   if(AlignFunc == NULL)
   {
      AlignFunc = AlignHook;
   }
   return;
}

/**
* @date     2012/01/03
*
*  Probe Passthrough : The original function is called direclty.
*
******************************************************************************/
void* MemAlignProbe::PassThrough(size_t i_Boundary, size_t i_Size)
{
   void *Data;
   memalign_t AlignFunc;

   AlignFunc = (memalign_t) rtsym_resolve("memalign");
   if(AlignFunc != NULL)
   {
      Data = AlignFunc(i_Boundary, i_Size);
   }
   return(Data);
}


/**
* @date     2012/01/03
*
*  Frees previously allocated data.
*
******************************************************************************/
void* MemAlignProbe::MemAlign(size_t i_Boundary, size_t i_Size, const CallStack& Callers)
{
   uint8_t  *Data = NULL;
   uint32_t Padding = MemProbe::GetAlignmentPadding(i_Boundary, sizeof(HeapEntry));

   if(AlignFunc != NULL)
   {
      if(GetHeap()->Lock() == 0)
      {
         Data = (uint8_t*)AlignFunc(i_Boundary, i_Size + sizeof(HeapEntry) + Padding);
         if(Data != NULL)
         {
            ExeContext *p_Context = ExeContext::Get(Callers);
            HeapEntry  *Entry = new (Data + Padding) HeapEntry(i_Size, p_Context);

            Entry->Start = Data;
            GetHeap()->GetEntryList()->AppendItem(Entry);
            if(p_Context != NULL)
               p_Context->UpdateMemory(i_Size);

            Data = Data + sizeof(HeapEntry) + Padding;
         }
      }
      else
      {
         Data = (uint8_t*)AlignFunc(i_Boundary, i_Size);
      }
      GetHeap()->Unlock();

   }
   return(Data);
}

