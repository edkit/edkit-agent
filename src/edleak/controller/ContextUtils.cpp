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
* @date     2012/02/16
*
*****************************************************************************/
#include "ExeContext.h"
#include "ContextUtils.h"


/**
* @date     2011/05/08
*
*  Build a new slice and the list of allocers in json strings.
*
* @param    p_JsonSlice (out): String that will contain the memory slice.
* @param    p_JsonAllocers (out): String that will contain the list of
*                                allocation points.
* @return   0 if success.
* @return   Error otherwise.
******************************************************************************/
int32_t CU_GetSlice(String *p_JsonSlice, String *p_JsonAllocers)
{
   if( (p_JsonSlice == NULL) || (p_JsonAllocers == NULL) )
      return(-1);

   p_JsonSlice->SetTo("");
   p_JsonAllocers->SetTo("");
   ExeContext  *p_CurContext =
      static_cast<ExeContext*>(ExeContext::GetList()->GetHead());

   uint32_t i_Index = 0;
   uint32_t i_AllocerIndex;
   bool     b_FirstContext = true;
   *p_JsonSlice << "[\n";
   *p_JsonAllocers << "\"allocer\": [\n";
   while(p_CurContext != NULL)
   {
      if(b_FirstContext == false)
      {
         *p_JsonSlice << ",\n";
         *p_JsonAllocers << ",\n";
      }
      else
         b_FirstContext = false;

      *p_JsonAllocers << "{ \"eip\": [";
      CallStack &Stack = p_CurContext->GetCallStack();
      uint32_t Depth = Stack.GetDepth();
      const char *CallerName;
      for(i_AllocerIndex=0; i_AllocerIndex<Depth; i_AllocerIndex++)
      {
         CallerName = Stack.GetName(i_AllocerIndex);
         if(CallerName != NULL)
         {
            if(i_AllocerIndex>0)
            {
               *p_JsonAllocers << ",";
            }
            *p_JsonAllocers << "\"" <<  CallerName << "\"";
         }
      }
      *p_JsonAllocers << "]}";

      *p_JsonSlice << "{ \"mem\":" << p_CurContext->Memory << ",";
      *p_JsonSlice << "\"alc\":" << i_Index << "}";

      p_CurContext = static_cast<ExeContext*>(p_CurContext->Next);
      i_Index++;
   }
   *p_JsonSlice << "\n]";
   *p_JsonAllocers << "\n]";

   return(0);
}
