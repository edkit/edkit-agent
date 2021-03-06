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
* @date     2012/01/25
*
*****************************************************************************/
#include "WsMethodSlice.h"
#include "ContextUtils.h"


/**
* @date     2012/01/25
*
*  Constructor.
*
******************************************************************************/
WsMethodSlice::WsMethodSlice():
   WsMethod("GetSlice")
{
   return;
}


/**
* @date     2012/01/25
*
*  Destructor.
*
******************************************************************************/
WsMethodSlice::~WsMethodSlice(void)
{
   return;
}


/**
* @date     2012/01/25
*
*  Slice Method : Returns a new memory slice.
*
******************************************************************************/
int32_t WsMethodSlice::Call(const DynObject &Params, String *p_Answer)
{
   int32_t i_Ret;
   String JsonSlice, JsonAllocers;

   if(p_Answer == NULL)
      return(-1);

   i_Ret = CU_GetSlice(&JsonSlice, &JsonAllocers);
   if(i_Ret == 0)
   {
      p_Answer->SetTo("{ \"status\": 0, \"data\" : {\n");

      *p_Answer << "\"slice\": [\n";
      *p_Answer << JsonSlice << "],\n";
      *p_Answer << JsonAllocers << "\n";
      *p_Answer << "} }\n";
   }

   return(i_Ret);
}
