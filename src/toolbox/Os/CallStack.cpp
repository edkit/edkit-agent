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
* @date     2011/05/01
*
*****************************************************************************/
#include <string.h>

#include "CallStack.h"


/**
* @date     2012/03/22
*
*  Constructor.
******************************************************************************/
CallStack::CallStack(void):
   Depth(CALLSTACK_MAX_DEPTH), SkipFrameCount(0), StackIndex(0)
{
   memset(Stack, 0, CALLSTACK_MAX_DEPTH*sizeof(void*));
   return;
}


/**
* @date     2012/03/22
*
*  Destructor.
*
******************************************************************************/
CallStack::~CallStack(void)
{
   return;
}


/**
* @date     2012/03/22
*
*  Equality operator. Two callstacks are considered equal if their Stack arrays
*  are identical.
*
******************************************************************************/
bool CallStack::operator==(const CallStack &Rhs) const
{
   if(memcmp(Stack, Rhs.Get(), Depth*sizeof(void*)) == 0)
      return(true);
   return(false);
}


/**
* @date     2012/02/22
*
*  Inequality operator.
*
******************************************************************************/
bool CallStack::operator!=(const CallStack &Rhs) const
{
   return(!operator==(Rhs));
}


/**
* @date     2012/03/22
*
*  Unwind the callers.
*
******************************************************************************/
void CallStack::Unwind(void)
{
   SkipFrameCount = 2;
   StackIndex = 0;
   _Unwind_Backtrace(UnwindCallback, this);
   return;
}


/**
* @date     2012/03/22
*
*  Unwind callback.
*
******************************************************************************/
_Unwind_Reason_Code  CallStack::UnwindCallback(struct _Unwind_Context *Context, void *Closure)
{
   CallStack *This = static_cast<CallStack*>(Closure);

   if(This != NULL)
      return(This->UnwindCallback(Context));
   return(_URC_NORMAL_STOP);
}


/**
* @date     2012/03/22
*
*  Unwind callback.
*
******************************************************************************/
_Unwind_Reason_Code  CallStack::UnwindCallback(struct _Unwind_Context *Context)
{
   if( (--SkipFrameCount < 0) && (StackIndex < CALLSTACK_MAX_DEPTH) )
   {
      Stack[StackIndex] = reinterpret_cast<void *>(_Unwind_GetIP(Context));
      StackIndex++;
   }

   return _URC_NO_REASON;
}


/**
* @date     2012/03/22
*
*  Sets the CallStack to the state of another one.
*
******************************************************************************/
void CallStack::SetTo(const CallStack &Target, uint32_t Level)
{
   memcpy(Stack, Target.Get(), sizeof(Stack));
   return;
}

