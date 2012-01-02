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
* @date     2011/12/22
*
*****************************************************************************/
#include "WsInterface.h"
#include "WsMethod.h"
#include "ListIterator.h"


/**
* @date     2011/12/22
*
*  Constructor.
*
******************************************************************************/
WsInterface::WsInterface(const String &Name):
   InitStatus(-1), InterfaceName(Name)
{
   return;
}


/**
* @date     2011/12/22
*
*  Destructor.
*
******************************************************************************/
WsInterface::~WsInterface(void)
{
   return;
}


/**
* @date     2011/12/22
*
*  Adds a method to this interface.
*
******************************************************************************/
int32_t  WsInterface::AddMethod(WsMethod *p_Method)
{
   if(p_Method == NULL)
      return(-1);

   return(MethodList.AddItem(p_Method));
}


/**
* @date     2011/12/22
*
*  Removes a method to this interface.
*
******************************************************************************/
int32_t  WsInterface::DelMethod(WsMethod *p_Method)
{
   if(p_Method == NULL)
      return(-1);

   return(MethodList.DelItem(p_Method));
}


/**
* @date     2011/12/22
*
*  Calls the method provided in Params.
*  The structure of Params is:
*  - String field "MethodName" : Method name.
*  - DynObj field "MethodParams" : Method parameters.
*
******************************************************************************/
int32_t  WsInterface::Call(const DynObject &Params, DynObject *p_Answer)
{
   ListIterator<AList<WsMethod*>, WsMethod*>  Iterator;
   WsMethod *p_Method = NULL;
   const String *MethodName;

   if( (Params.FindString("MethodName", &MethodName) != 0) ||
       (MethodName == NULL) )
   {
      return(-1);
   }

   Iterator.SetTo(&MethodList);
   Iterator.First();
   while(Iterator.IsDone() == false)
   {
      if( (Iterator.GetItem(&p_Method) == 0) && (p_Method != NULL) )
      {
         if(strcmp(p_Method->GetName().GetString(), MethodName->GetString()) == 0)
         {
            const DynObject *p_Params = NULL;

            if( (Params.FindDynObject("MethodParams", &p_Params) == 0) &&
                  (p_Params != NULL) )
            {
               return(p_Method->Call(*p_Params, p_Answer));
            }
         }
      }
      Iterator.Next();
   }
   return(-1);
}
