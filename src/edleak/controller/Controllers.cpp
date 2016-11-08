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
* @date     2011/05/11
*
*****************************************************************************/
#include <new>
#include <stdlib.h>

#include <unistd.h>
#include <pthread.h>

#include "Controllers.h"
#include "WebService.h"

static   WebService  *p_WebService = NULL;

static void AtforkPrepare(void);
static void AtforkChild(void);
static void ControllersInstantiate(void);

void AtforkPrepare(void)
{
    ControllersEnd();
    /** @bug
        There are still some threads doing allocations after the webservice
        controller exits, which causes a deadlock.
        We wait a little bit for now until a fix is found.
    */
    sleep(1);
}

void AtforkChild(void)
{
   ControllersInstantiate();
}

void ControllersInit(void)
{
    ControllersInstantiate();
    pthread_atfork(AtforkPrepare, NULL, AtforkChild);
    atexit(ControllersEnd);
}

void ControllersInstantiate(void)
{
   if(p_WebService == NULL)
      p_WebService = new(std::nothrow) WebService();
}

void ControllersEnd(void)
{
  if(p_WebService != NULL)
  {
      delete p_WebService;
      p_WebService = NULL;
  }
}
