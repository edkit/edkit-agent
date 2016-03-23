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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <dlfcn.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cxxabi.h>
#include "CallStack.h"

#ifdef HAVE_LIBUNWIND
#define UNW_LOCAL_ONLY
#include "libunwind.h"
#endif

/**
* @date     2012/03/22
*
*  Constructor.
******************************************************************************/
CallStack::CallStack(void):
   Depth(CALLSTACK_MAX_DEPTH), SkipFrameCount(0), StackIndex(0),
   NamesAreResolved(false)
{
   memset(Stack, 0, CALLSTACK_MAX_DEPTH*sizeof(void*));
   memset(StackNames, 0, CALLSTACK_MAX_DEPTH*ALLOCER_NAME_SIZE*sizeof(char));
   return;
}


/**
* @date     2012/02/09
*
*  Copy Constructor.
*  @param Callers (in): Callstack.to copy.
*  @param Level (in): Optionnal max level to copy. If Level is strictly positive
*  and less that the depth of Callers, then only this level of callers is
*  assigned.
******************************************************************************/
CallStack::CallStack(const CallStack& Callers, uint32_t Level):
   Depth(Callers.GetDepth()), SkipFrameCount(0), StackIndex(0),
   NamesAreResolved(false)
{
   memset(Stack, 0, CALLSTACK_MAX_DEPTH*sizeof(void*));
   memset(StackNames, 0, CALLSTACK_MAX_DEPTH*ALLOCER_NAME_SIZE*sizeof(char));
   if( (Level > 0) &&(Level < Depth) )
      Depth = Level;
   SetTo(Callers);
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
* @date     2013/02/22
*
*  Equality operator. Two callstacks are considered equal if their Stack arrays
*  are identical.
*
******************************************************************************/
bool CallStack::operator==(const CallStack &Rhs) const
{
   if(Depth == Rhs.GetDepth())
   {
      if(memcmp(Stack, Rhs.Get(), Depth*sizeof(void*)) == 0)
         return(true);
   }
   return(false);
}


/**
* @date     2013/02/22
*
*  Inequality operator.
*
******************************************************************************/
bool CallStack::operator!=(const CallStack &Rhs) const
{
   return(!operator==(Rhs));
}


/**
* @date     2013/03/25
*
*  Equality operator for only a specified depth.
******************************************************************************/
bool CallStack::CallerIs(const CallStack &Rhs, uint32_t Level) const
{
   if((Depth >= Level) && (Rhs.GetDepth() >= Level))
   {
      if(memcmp(Stack, Rhs.Get(), Level*sizeof(void*)) == 0)
         return(true);
   }
   return(false);
}


#ifdef HAVE_LIBUNWIND
void CallStack::Unwind(void)
{
   Depth = CALLSTACK_MAX_DEPTH;
   SkipFrameCount = 1;
   StackIndex = 0;

   unw_cursor_t cursor;
   unw_context_t context;

   // Initialize cursor to current frame for local unwinding.
   unw_getcontext(&context);
   unw_init_local(&cursor, &context);

   // Unwind frames one by one, going up the frame stack.
   while (unw_step(&cursor) > 0) {
      unw_word_t pc;
      unw_get_reg(&cursor, UNW_REG_IP, &pc);
      if (pc == 0) {
         break;
      }

      if( (--SkipFrameCount < 0) && (StackIndex < CALLSTACK_MAX_DEPTH) )
      {
         Stack[StackIndex] = reinterpret_cast<void *>(pc);
         StackIndex++;
      }
   }
   return;
}

#else
/**
* @date     2012/03/22
*
*  Unwind the callers.
*
******************************************************************************/
void CallStack::Unwind(void)
{
   Depth = CALLSTACK_MAX_DEPTH;
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
   return _URC_FOREIGN_EXCEPTION_CAUGHT;
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
#endif

/**
* @date     2013/02/25
*
*  Initializes the callstack from another callstack but for only one level (the
*  caller).
*
******************************************************************************/
void CallStack::SetCaller(const CallStack& Callers)
{
   Depth = 1;
   Stack[0] = Callers.Get()[0];
   return;
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


/**
* @date     2013/02/11
*
*  Resolves the symbols names in the callstack.
*
******************************************************************************/
void CallStack::ResolveNames(void)
{
   if(NamesAreResolved == true)
      return;

   uint32_t Level;
   for(Level=0; Level<Depth; Level++)
   {
      if(Stack[Level] != NULL)
      {
         Dl_info s_info;
         if( (dladdr(Stack[Level], &s_info) != 0) && (s_info.dli_sname != NULL) )
         {
            int Status;
            char* UnmangledName =
               abi::__cxa_demangle(s_info.dli_sname, NULL, 0, &Status);
            if(UnmangledName  != NULL)
            {
               snprintf(StackNames[Level], ALLOCER_NAME_SIZE, "%p:%s",
                  Stack[Level], UnmangledName);
               free(UnmangledName);
            }
            else
            {
               snprintf(StackNames[Level], ALLOCER_NAME_SIZE, "%p:%s",
                  Stack[Level], s_info.dli_sname);
            }
            if(s_info.dli_fname != NULL)
            {
               strncpy(SoNames[Level], s_info.dli_fname, ALLOCER_NAME_SIZE);
            }
            else
            {
               strncpy(SoNames[Level], "", ALLOCER_NAME_SIZE);
            }
         }
         else
         {
            snprintf(StackNames[Level], ALLOCER_NAME_SIZE, "%p", Stack[Level]);
         }
         StackNames[Level][ALLOCER_NAME_SIZE-1] = '\0';
         SoNames[Level][ALLOCER_NAME_SIZE-1] = '\0';
      }
   }
   NamesAreResolved = true;
}


/**
* @date     2013/02/09
*
*  Returns the symbol name of the callstack at the specified level.
*
******************************************************************************/
const char* CallStack::GetName(uint32_t Level)
{
   if(Level >= Depth)
      return(NULL);

   ResolveNames();
   if(StackNames[Level][0] == '\0')
      return(NULL);
   return(StackNames[Level]);
}

/**
*  Returns the shared object name of the callstack at the specified level.
*/
const char* CallStack::GetSoName(uint32_t Level)
{
   if(Level >= Depth)
      return(NULL);

   ResolveNames();
   if(SoNames[Level][0] == '\0')
      return(NULL);
   return(SoNames[Level]);
}
