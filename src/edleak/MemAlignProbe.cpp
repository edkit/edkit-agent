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
void* MemAlignProbe::MemAlign(size_t i_Boundary, size_t i_Size, void *Eip)
{
   uint8_t  *Data = NULL;
   if(AlignFunc != NULL)
   {
      if(GetHeap()->Lock() == 0)
      {
         /// @todo fix boundary according to HeapEntry size
         Data = (uint8_t*)AlignFunc(i_Boundary, i_Size + sizeof(HeapEntry));
         if(Data != NULL)
         {
            ExeContext *p_Context = ExeContext::Get(Eip);
            HeapEntry  *Entry = new (Data) HeapEntry(i_Size, p_Context);

            GetHeap()->GetEntryList()->AppendItem(Entry);
            if(p_Context != NULL)
               p_Context->Memory += i_Size;

            Data = Data + sizeof(HeapEntry);
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

