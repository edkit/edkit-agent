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
#include <new>
#include <dlfcn.h>
#include <stdio.h>

#include "ExeContext.h"


/**
* @date     2011/05/01
*
*  Constructor.
*
******************************************************************************/
ExeContext::ExeContext(const CallStack &Callers, uint32_t Level):
   DlListItem(),
   Memory(0), Stack(Callers, Level), ParentContext(NULL), Id(0)

{
   SetId();
   return;
}


/**
* @date     2011/05/01
*
*  Destructor.
*
******************************************************************************/
ExeContext::~ExeContext()
{
   return;
}


/**
* @date     2013/05/22
*
*  resets all ExeContexts.
*  Warning : This should be used in unit tests only.
******************************************************************************/
void ExeContext::Reset(void)
{
   DlList *p_ContextList = ExeContext::GetList();
   ExeContext *p_Context;

   while(1)
   {
      p_Context = static_cast<ExeContext*>(p_ContextList->GetHead());
      if(p_Context != NULL)
      {
         p_ContextList->DelItem(p_Context);
         delete p_Context;
      }
      else
      {
         break;
      }
   }
}

/**
* @date     2013/03/10
*
*  Sets a unique Id assotiated to a context.
******************************************************************************/
void ExeContext::SetId(void)
{
   static uint32_t NextId = 0;
   /** @todo :
     - atomic
     - check against integer overflow
     - check against already used id
   */
   Id = NextId++;
}


/**
* @date     2011/05/01
*
*  returns the first element of the context list.
*
* @return   List of contexts.
*
******************************************************************************/
DlList* ExeContext::GetList(void)
{
   static DlList ContextList;

   return(&ContextList);
}


/**
* @date     2011/05/01
*
*  returns an ExeContext object. if a context already exists for the specified
*  callers, then this context is returned. Otherwise a new context is created and
*  returned.
*
* @param    Callers (in): Callstack of the callers.
* @return   ExeContext if success.
* @return   NULL otherwise.
*
******************************************************************************/
ExeContext* ExeContext::Get(const CallStack &Callers)
{
   DlList *p_ContextList = ExeContext::GetList();
   ExeContext *p_Context = static_cast<ExeContext*>(p_ContextList->GetHead());
   ExeContext *p_ParentContext = NULL;

   /* search for an existing allocer */
   while(p_Context != NULL)
   {
      if(p_Context->GetCallStack() == Callers)
         break;
      else if( (p_ParentContext == NULL) &&
               (p_Context->GetCallStack().GetDepth() == 1) &&
               (p_Context->GetCallStack().CallerIs(Callers)) )
      {
         p_ParentContext = p_Context;
      }
      p_Context = (ExeContext*)p_Context->Next;
   }

   /* allocate a new entry if not found */
   if(p_Context == NULL)
   {
      p_Context = new(std::nothrow) ExeContext(Callers);
      if(p_Context != NULL)
      {
         if(Callers.GetDepth() == 1)
         {
            p_ContextList->AppendItem(p_Context);
         }
         else
         {
            /* And also allocate parent if it does not exist yet */
            if(p_ParentContext == NULL)
            {
               p_ParentContext = new(std::nothrow) ExeContext(Callers, 1);
               if(p_ParentContext != NULL)
                  p_ContextList->AppendItem(p_ParentContext);
            }
            if(p_ParentContext != NULL)
            {
               p_Context->ParentContext = p_ParentContext;
               p_ContextList->AppendItem(p_Context);
            }
            else
            {
               delete p_Context;
               p_Context = NULL;
            }
         }
      }
   }
   return(p_Context);
}


/**
* @date     2013/03/10
*
*  returns an ExeContext object that matches the provided Id.
*
* @param    Id (in): Id to search.
* @return   ExeContext if success.
* @return   NULL otherwise.
******************************************************************************/
ExeContext* ExeContext::Get(const uint32_t Id)
{
   DlList *p_ContextList = ExeContext::GetList();
   ExeContext *p_Context = static_cast<ExeContext*>(p_ContextList->GetHead());

   while(p_Context != NULL)
   {
      if(p_Context->GetId() == Id)
         break;
      p_Context = (ExeContext*)p_Context->Next;
   }

   return(p_Context);
}

