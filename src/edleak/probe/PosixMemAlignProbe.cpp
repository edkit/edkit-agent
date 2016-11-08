/*
*****************************************************************************
*                      ___       _   _    _ _
*                     / _ \ __ _| |_| |__(_) |_ ___
*                    | (_) / _` | / / '_ \ |  _(_-<
*                     \___/\__,_|_\_\_.__/_|\__/__/
*                          Copyright (c) 2016
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
* @date     2016/11/08
*
*****************************************************************************/
#include <new>
#include <errno.h>
#include "PosixMemAlignProbe.h"
#include "ExeContext.h"
#include "HeapEntry.h"


PosixMemAlignProbe::PosixMemAlignProbe(posix_memalign_t AlignHook):
   MemProbe(),
   AlignFunc(AlignHook)
{
   return;
}


PosixMemAlignProbe::~PosixMemAlignProbe()
{
   return;
}


int PosixMemAlignProbe::MemAlign(void ** Memptr, size_t i_Alignment, size_t i_Size, const CallStack& Callers)
{
   if(Memptr == NULL)
      return EINVAL;

   int i_Ret = 0;
   uint8_t  *Data = NULL;
   uint32_t Padding = MemProbe::GetAlignmentPadding(i_Alignment, sizeof(HeapEntry));

   if(AlignFunc != NULL)
   {
      if(GetHeap()->Lock() == 0)
      {
         i_Ret = AlignFunc((void**)&Data, i_Alignment, i_Size + sizeof(HeapEntry) + Padding);
         if( (i_Ret == 0) && (Data != NULL) )
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
         i_Ret = AlignFunc((void**)&Data, i_Alignment, i_Size);
      }
      GetHeap()->Unlock();

   }

   *Memptr = Data;
   return(i_Ret);
}
