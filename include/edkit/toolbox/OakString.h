#ifndef STRING_H
#define STRING_H
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
* @date     2011/12/19
*
*****************************************************************************/
#include "stdint.h"
#include "stdlib.h"
#include "string.h"

class String
{
   public:
                           String(void);
                           String(const String &BaseString);
                           String(const char*BaseString, uint32_t i_Length = 0);
      virtual              ~String(void);
               int32_t     InitCheck(void) const { return(InitStatus); };

               bool        operator==(const String& Rhs) const;
               bool        operator!=(const String& Rhs) const;
               String&     operator<<(int Rhs);
               String&     operator<<(const char* Rhs);
               String&     operator<<(const String& Rhs);

               const char* GetString(void) const
                  {
                     if(Area != NULL)
                        return(Area);
                     else
                        return "";
                  };
               uint32_t    GetSize(void) const { return(strlen(Area));};
               int32_t     SetTo(const String &Value);
               int32_t     SetTo(const char *Value, uint32_t i_Length);
               int32_t     FindFirst(const String &SearchString) const ;

      private:
               void        DelArea(void);
               int32_t     AppendString(const char* sz_String, uint32_t i_Length);

               int32_t     InitStatus;
               char*       Area;
               uint32_t    AreaSize;
};

#endif /* STRING_H */
