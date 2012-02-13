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
ExeContext::ExeContext(void): DlListItem()
{
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
*  eip, then this context is returned. Otherwise a new context is created and
*  returned.
*
* @param    ContextEip (in): Address of the caller.
* @return   ExeContext if success.
* @return   NULL otherwise.
*
******************************************************************************/
ExeContext* ExeContext::Get(void *ContextEip)
{
   DlList *p_ContextList = ExeContext::GetList();
   ExeContext *p_Context = static_cast<ExeContext*>(p_ContextList->GetHead());

   /* search for an existing allocer */
   while(p_Context != NULL)
   {
      if(p_Context->Eip == ContextEip)
         break;
      p_Context = (ExeContext*)p_Context->Next;
   }

   /* allocate a new entry if not found */
   if(p_Context == NULL)
   {
      p_Context = new(std::nothrow) ExeContext();
      if(p_Context != NULL)
      {
         Dl_info s_info;

         p_Context->Eip = ContextEip;
         p_Context->Memory = 0;
         if( (dladdr(ContextEip, &s_info) != 0) && (s_info.dli_sname != NULL) )
         {
            snprintf(p_Context->Name, ALLOCER_NAME_SIZE, "%p:%s",
                  ContextEip, s_info.dli_sname);
         }
         else
         {
            snprintf(p_Context->Name, ALLOCER_NAME_SIZE, "%p", ContextEip);
         }
         p_Context->Name[ALLOCER_NAME_SIZE-1] = '\0';
         p_ContextList->AppendItem(p_Context);
      }
   }
   return(p_Context);
}


