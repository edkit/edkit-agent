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
* @date     2013/03/04
*
*****************************************************************************/
#include <stdio.h>
#include "WsMethodStackWatchAdd.h"
#include "ContextCallStackList.h"


/**
* @date     2013/03/04
*
*  Constructor.
******************************************************************************/
WsMethodStackWatchAdd::WsMethodStackWatchAdd():
   WsMethod("AddStackWatch")
{
   return;
}


/**
* @date     2013/03/04
*
*  Destructor.
******************************************************************************/
WsMethodStackWatchAdd::~WsMethodStackWatchAdd(void)
{
   return;
}


/**
* @date     2013/03/04
*
*  Add StackWatch Method : Adds a new stackcall context. The provided caller eip
*  will then be monitored with the full call stack.
******************************************************************************/
int32_t WsMethodStackWatchAdd::Call(const DynObject &Params, String *p_Answer)
{
   int32_t i_Ret;
   String JsonSlice, JsonAllocers;

   if(p_Answer == NULL)
      return(-1);

   ContextCallStackList *CallStackList = ContextCallStackList::Instantiate();
   if(CallStackList == NULL)
      return(-1);

   int64_t  ContextId;
   i_Ret = Params.FindInt("id", &ContextId);
   if(i_Ret == 0)
   {
      ExeContext *Context = ExeContext::Get(ContextId);
      if(Context == NULL)
         i_Ret = -1;
      if(i_Ret == 0)
      {
         const void *Caller = Context->GetCallStack().Get()[0];
         if(CallStackList->HasItem(Caller) == false)
         {
            i_Ret = CallStackList->AddItem(Caller);
         }

         if(i_Ret == 0)
         {
            p_Answer->SetTo("{ \"status\": 0, \"data\" : {} }\n");
         }
      }
   }

   return(i_Ret);
}
