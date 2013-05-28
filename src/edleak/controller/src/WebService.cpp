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
* @date     2012/01/24
*
*****************************************************************************/
#include <new>
#include "WebService.h"
#include "WsInterface.h"
#include "WsMethodSlice.h"
#include "WsMethodStackWatchAdd.h"
#include "WsHandlerSingleton.h"

/**
* @date     2012/01/24
*
*  Constructor.
******************************************************************************/
WebService::WebService(void):
   Interface(NULL), SliceMethod(NULL), StackWatchAddMethod(NULL)
{
   int32_t i_Ret;

   WsHandlerSingleton *WsHandler = WsHandlerSingleton::Instantiate();
   if(WsHandler == NULL)
      return;

   Interface = new(std::nothrow) WsInterface("Edleak");
   if(Interface == NULL)
      return;
   i_Ret = Interface->InitCheck();
   if(i_Ret == 0)
   {
      SliceMethod = new(std::nothrow) WsMethodSlice();
      if(SliceMethod != NULL)
      {
         Interface->AddMethod(SliceMethod);
         StackWatchAddMethod = new(std::nothrow) WsMethodStackWatchAdd();
         if(StackWatchAddMethod != NULL)
         {
            Interface->AddMethod(StackWatchAddMethod);
         }
      }
   }
   WsHandler->AddInterface(Interface);
   return;
}


/**
* @date     2012/01/24
*
*  Destructor.
*
******************************************************************************/
WebService::~WebService(void)
{
   if(Interface != NULL)
      delete Interface;
   return;
}


