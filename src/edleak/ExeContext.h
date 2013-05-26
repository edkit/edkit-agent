#ifndef __EXECONTEXT_H
#define __EXECONTEXT_H

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
#include <stdint.h>
#include "DlList.h"
#include "CallStack.h"

class ExeContext : public DlListItem
{
   public:
                              ExeContext(const CallStack &Callers, uint32_t Level=0);
      virtual                 ~ExeContext(void);
      static   void           Reset(void);
      static   ExeContext*    Get(const CallStack &Callers);
      static   ExeContext*    Get(const uint32_t Id);
               uint32_t       GetId(void) const {return(Id);};
      static   DlList*        GetList(void);
               CallStack&     GetCallStack(void) { return(Stack);};
               void           UpdateMemory(size_t Size)
               {
                  Memory += Size;
                  if(ParentContext != NULL)
                     ParentContext->UpdateMemory(Size);
               };
               int64_t        GetMemory(void) const {return(Memory);};

   private:
                              ExeContext(void);
               void           SetId(void);

               int64_t        Memory;
               CallStack      Stack;
               ExeContext     *ParentContext;
               uint32_t       Id;
};

#endif
