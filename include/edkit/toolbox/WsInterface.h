#ifndef WSINTERFACE_H
#define WSINTERFACE_H
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
#include <stdint.h>
#include "OakString.h"
#include "AList.h"
#include "DynObject.h"

class WsMethod;
class WsInterface
{
   public:
                              WsInterface(const String &Name);
      virtual                 ~WsInterface(void);
               int32_t        InitCheck(void) const { return(InitStatus); };

               const String&  GetName(void) const { return(InterfaceName); };
               int32_t        AddMethod(WsMethod *p_Method);
               int32_t        DelMethod(WsMethod *p_Method);
      virtual  int32_t        Call(const DynObject &Params, String *p_Answer);


      protected:
               int32_t           InitStatus;

      private:
               const String      InterfaceName;
               AList<WsMethod*>  MethodList;
};

#endif /* WSINTERFACE_H */
