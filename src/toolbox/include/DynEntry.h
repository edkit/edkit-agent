#ifndef DYNENTRY_H
#define DYNENTRY_H
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

class DynEntryBase
{  
   public:
                           DynEntryBase(void) {};
      virtual              ~DynEntryBase(void) {};
};

template <class T>
class DynEntry : public DynEntryBase
{
   public:
#define  NAME_SIZE   64
                           DynEntry(const char *EntryName, T &Value) : 
                              DynEntryBase(),
                              Name(), Value(Value), InitStatus(-1)
                           { 
                              if(EntryName != NULL)
                              {
                                 if(Name.SetTo(EntryName) == 0)
                                    InitStatus = 0;
                              }

                           };

      virtual              ~DynEntry(void);

               int32_t     InitCheck(void) const { return(InitStatus); };

               const char* GetName(void) const {return(Name.GetString());};
      virtual  T           GetValue(void) const {return(Value);};
      virtual  T*          GetValuePtr(void) {return(&Value);};

      private:
               String      Name;
               T           Value;
               int32_t     InitStatus;

      
};

#endif /* DYNENTRY_H */
