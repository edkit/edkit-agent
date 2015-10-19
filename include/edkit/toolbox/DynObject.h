#ifndef DYNOBJECT_H
#define DYNOBJECT_H
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
#include "AList.h"
#include "DynEntry.h"

class String;
class DynObject
{
   public:
                                 DynObject(void);
                                 DynObject(const DynObject &Object);
      virtual                    ~DynObject(void);
               int32_t           InitCheck(void) const { return(InitStatus); };

               int32_t           AddInt(const char *Name, int64_t Value);
               int32_t           AddString(const char *Name, const String &Value);
               int32_t           AddDynObject(const char *Name, const DynObject &Value);

               int32_t           FindInt(const char *Name, int64_t *Value) const;
               int32_t           FindString(const char *Name, const String **Value) const ;
               int32_t           FindDynObject(const char *Name, const DynObject **Value) const ;

               DynEntryBase*     GetEntryAt(uint32_t i_Index) const;
               bool              HasEntry(const char *Name) const;
               int32_t           MakeEmpty(void);

   private:
               int32_t                 InitStatus;
               AList<DynEntryBase*>    EntryList;
};

#endif /* DYNENTRY_H */
