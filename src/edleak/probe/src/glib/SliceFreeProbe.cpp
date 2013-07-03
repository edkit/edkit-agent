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
* @date     2013/06/02
*
*****************************************************************************/
#include <new>
#include "SliceFreeProbe.h"
#include "HeapEntry.h"
#include "rtsym.h"

namespace GLib {

/**
* @date     2013/06/02
*
*  Constructor.
*
******************************************************************************/
SliceFreeProbe::SliceFreeProbe(void):
   MemProbe(),
   FreeFunc(NULL)
{
   return;
}


/**
* @date     2013/06/02
*
*  Destructor.
*
******************************************************************************/
SliceFreeProbe::~SliceFreeProbe()
{
   return;
}


/**
* @date     2013/06/02
*
*  Probe initialization.
*
* @param sz_FreeFunc (in): Name of the deallocation function. "g_slice_free1" is
*                          used if NULL is provided here.
*
******************************************************************************/
void SliceFreeProbe::InitCheck(const char *sz_FreeFunc)
{
   if(FreeFunc == NULL)
   {
      if(sz_FreeFunc != NULL)
         FreeFunc = (slice_free_t) rtsym_resolve(sz_FreeFunc);
      else
         FreeFunc = (slice_free_t) rtsym_resolve("g_slice_free1");
   }
   return;
}


/**
* @date     2013/06/02
*
*  Probe Passthrough : The original function is called direclty.
*
******************************************************************************/
void SliceFreeProbe::PassThrough(gsize Size, gpointer Data, const char *sz_FreeFunc)
{
   slice_free_t FreeFunc;

   if(sz_FreeFunc != NULL)
      FreeFunc = (slice_free_t) rtsym_resolve(sz_FreeFunc);
   else
      FreeFunc = (slice_free_t) rtsym_resolve("g_slice_free1");

   if(FreeFunc != NULL)
   {
      FreeFunc(Size, Data);
   }
   return;
}


/**
* @date     2013/06/02
*
*  Frees previously allocated data.
*
******************************************************************************/
void SliceFreeProbe::Free(gsize Size, gpointer Data)
{
   if(FreeFunc != NULL)
   {
      if(GetHeap()->Lock() == 0)
      {
         if(Data != NULL)
         {
            HeapEntry *Entry = (HeapEntry*)((uint8_t*)Data - sizeof(HeapEntry));
            if(Entry->Magic == HEAPENTRY_MAGIC)
            {
               GetHeap()->GetEntryList()->DelItem(Entry);
               if(Entry->Start != NULL) // there is some padding on allocated chunk
                  Data = Entry->Start;
               else
                  Data = Entry;
               Entry->Magic = 0;   /* sanity clean */
               if(Entry->Context != NULL)
                  Entry->Context->UpdateMemory(-(Entry->Size));
               Size += sizeof(HeapEntry);
            }
         }
      }

      FreeFunc(Size, Data);
      GetHeap()->Unlock();

   }
   return;
}

} // GLib
