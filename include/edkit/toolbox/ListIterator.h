#ifndef LISTITERATOR_H
#define LISTITERATOR_H
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

template <class T, class S>
class ListIterator
{
   public:
                           ListIterator(void) : List(NULL), CurrentIndex(0) {};
      virtual              ~ListIterator(void) {};

               int32_t     SetTo(const T *p_List)
                           {
                              if(p_List == NULL)
                                 return(-1);
                              List = p_List;
                              CurrentIndex = 0;
                              return(0);
                           };
               int32_t     First(void) { CurrentIndex = 0; return(0);};
               int32_t     Next(void) { CurrentIndex++; return(0);};
               bool        IsDone(void) const
                           {
                              if(List == NULL)
                                 return(true);
                              if(CurrentIndex >= List->CountItems())
                                 return(true);
                              return(false);
                           };

               int32_t     GetItem(S *p_Item)
                           {
                              if( (List == NULL) || (p_Item == NULL) )
                                 return(-1);

                              if(List->GetItemAt(CurrentIndex, p_Item) == 0)
                                 return(0);
                              return(-1);
                           };

      private:
               const T     *List;
               uint32_t    CurrentIndex;
};

#endif /* ALIST_H */
