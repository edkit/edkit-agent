#ifndef RTSYM_H
#define RTSYM_H

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
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void *(*malloc_t)(size_t);
typedef void *(*calloc_t)(size_t nmemb, size_t size);
typedef void *(*realloc_t)(void*, size_t);
typedef void (*free_t)(void *);
typedef void *(*memalign_t)(size_t boundary, size_t size);

extern void* rtsym_resolve(const char *sz_symbol);

#ifdef __cplusplus
}
#endif

#if __GXX_ABI_VERSION == 1002
#define CXX_SYM_NEW              _Znwj
#define CXX_SYM_NEW_NOTHROW      _ZnwjRKSt9nothrow_t
#define CXX_SYM_DELETE_NOTHROW   _ZdlPvRKSt9nothrow_t
#define CXX_SYM_DELETE           _ZdlPv
#else
#error Unsupported ABI
#endif

#define SYM_STRING(S) #S
#define M2STR(S) SYM_STRING(S)

#endif /* RTSYM_H */
