#ifndef __CALLSTACK_H
#define __CALLSTACK_H

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
* @date     2012/03/22
*
*****************************************************************************/
#include <stdint.h>
#include <unwind.h>

class CallStack
{
   public:
#define CALLSTACK_MAX_DEPTH         5

                                    CallStack(void);
                                    ~CallStack(void);

               bool                 operator==(const CallStack &Rhs) const;
               bool                 operator!=(const CallStack &Rhs) const;


               void                 Unwind(void);
               void                 UnwindCaller(void) { Stack[0] = __builtin_return_address(0); };
               const void**         Get(void) const { return((const void**)(Stack)); };
               uint32_t             GetDepth(void) const { return(Depth);};
               void                 SetTo(const CallStack &Target, uint32_t Level = CALLSTACK_MAX_DEPTH);

   private:
      static   _Unwind_Reason_Code  UnwindCallback(struct _Unwind_Context *Context, void *Closure);
               _Unwind_Reason_Code  UnwindCallback(struct _Unwind_Context *Context);

               void*                Stack[CALLSTACK_MAX_DEPTH];
               uint32_t             Depth;
               int32_t              SkipFrameCount;
               uint32_t             StackIndex;
};

#endif
