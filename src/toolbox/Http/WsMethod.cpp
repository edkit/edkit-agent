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
* @date     2011/12/21
*
*****************************************************************************/
#include "WsMethod.h"
#include "JsonFlattener.h"


/**
* @date     2011/12/21
*
*  Constructor.
*
******************************************************************************/
WsMethod::WsMethod(const String &Name):
   InitStatus(0), MethodName(Name)
{
   return;
}


/**
* @date     2011/12/21
*
*  Destructor.
*
******************************************************************************/
WsMethod::~WsMethod(void)
{
   return;
}


/**
* @date     2012/02/22
*
*  Call the method. Overload it for method that return informations directly in
*  a json string.
*
* @param    Params (in): Method call parameters.
* @param    p_Answer (out): Method return values.
* @return   0 if success.
* @return   Error otherwise.
******************************************************************************/
int32_t WsMethod::Call(const DynObject &Params, String *p_Answer)
{
   int32_t     i_Ret;
   DynObject   ObjAnswer;

   i_Ret = Call(Params, &ObjAnswer);
   if(i_Ret == 0)
   {
      JsonFlattener Flattener;
      i_Ret = Flattener.Flatten(&ObjAnswer, p_Answer);
   }
   return(i_Ret);
}


/**
* @date     2012/02/22
*
*  Call the method. Overload it for method that return informations in a
*  DynObject.
*
* @param    Params (in): Method call parameters.
* @param    p_Answer (out): Method return values.
* @return   0 if success.
* @return   Error otherwise.
******************************************************************************/
int32_t WsMethod::Call(const DynObject &Params, DynObject *p_Answer)
{
   return(-1);
}

