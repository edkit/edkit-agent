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
* @date     2016/11/02
*****************************************************************************/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_JEMALLOC
#include "jemalloc/jemalloc.h"
#endif
#include "PosixSymbol.h"

#define PREFIX JEMALLOC_PREFIX
#define PASTER(x,y) x ## y
#define EVALUATOR(x,y)  PASTER(x,y)
#define JEMALLOC_SYM(symbol) EVALUATOR(PREFIX, symbol)

malloc_t PosixSymbol::malloc()
{
    static malloc_t mallocSymbol = NULL;

    if(mallocSymbol == NULL)
    {
#ifdef HAVE_JEMALLOC
        mallocSymbol = JEMALLOC_SYM(malloc);
#else
        mallocSymbol = (malloc_t) rtsym_resolve("malloc");
#endif
    }

    return mallocSymbol;
}

malloc_t PosixSymbol::new_throw()
{
    static malloc_t mallocSymbol = NULL;

    if(mallocSymbol == NULL)
    {
#ifdef HAVE_JEMALLOC
        mallocSymbol = JEMALLOC_SYM(malloc);
#else
        mallocSymbol = (malloc_t) rtsym_resolve(CXX_SYM_NEW);
#endif
    }

    return mallocSymbol;
}

malloc_t PosixSymbol::new_nothrow()
{
    static malloc_t mallocSymbol = NULL;

    if(mallocSymbol == NULL)
    {
#ifdef HAVE_JEMALLOC
        mallocSymbol = JEMALLOC_SYM(malloc);
#else
        mallocSymbol = (malloc_t) rtsym_resolve(CXX_SYM_NEW_NOTHROW);
#endif
    }

    return mallocSymbol;
}


calloc_t PosixSymbol::calloc()
{
    static calloc_t callocSymbol = NULL;

    if(callocSymbol == NULL)
    {
#ifdef HAVE_JEMALLOC
        callocSymbol = JEMALLOC_SYM(calloc);
#else
    #if defined(__GLIBC__) && !defined(__UCLIBC__) // uclibc also defines glibc.
        /* dlsym calls calloc in GLIBC, so we get its internal name instead of
         * resolving it dynamically */
        callocSymbol = (calloc_t)__libc_calloc;
    #else
        callocSymbol = (calloc_t)rtsym_resolve("calloc");
    #endif
#endif
    }

    return callocSymbol;
}

realloc_t PosixSymbol::realloc()
{
    static realloc_t reallocSymbol = NULL;

    if(reallocSymbol == NULL)
    {
#ifdef HAVE_JEMALLOC
        reallocSymbol = JEMALLOC_SYM(realloc);
#else
        reallocSymbol = (realloc_t) rtsym_resolve("realloc");
#endif
    }

    return reallocSymbol;
}

free_t PosixSymbol::free()
{
    static free_t freeSymbol = NULL;

    if(freeSymbol == NULL)
    {
#ifdef HAVE_JEMALLOC
        freeSymbol = JEMALLOC_SYM(free);
#else
        freeSymbol = (free_t) rtsym_resolve("free");
#endif
    }

    return freeSymbol;
}

memalign_t PosixSymbol::memalign()
{
    static memalign_t memalignSymbol = NULL;

    if(memalignSymbol == NULL)
    {
#ifdef HAVE_JEMALLOC
        memalignSymbol = JEMALLOC_SYM(memalign);
#else
        memalignSymbol = (realloc_t) rtsym_resolve("memalign");
#endif
    }

    return memalignSymbol;
}
