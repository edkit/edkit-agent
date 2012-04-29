#ifndef __FAKEALLOC_H_
#define __FAKEALLOC_H_

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
* @date     2012/04/29
*
*****************************************************************************/
#include "rtsym.h"

extern "C"
{
extern void *FakeAlloc_Malloc (size_t i_Size) throw();
extern void *FakeAlloc_Memalign(size_t i_Boundary, size_t i_Size) throw();
extern void *FakeAlloc_Calloc(size_t i_MembCount, size_t i_Size) throw();
extern void *FakeAlloc_Realloc(void *Ptr, size_t i_Size) throw();
extern void FakeAlloc_Free (void *Ptr) throw();

extern void *FakeAlloc_New(size_t i_Size);
extern void *FakeAlloc_NewNoThrow(size_t i_Size)  throw();
extern void FakeAlloc_DeleteNoThrow(void *Ptr) throw();
extern void FakeAlloc_Delete(void *Ptr);

extern void FakeAlloc_SetAllocAddress(void *Address);
extern void *FakeAlloc_GetFreeAddress(void);
}
#endif
