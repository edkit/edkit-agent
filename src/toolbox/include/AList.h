#ifndef ALIST_H
#define ALIST_H
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
#include <stdlib.h>
#include <string.h>

template <class T>
class AList
{
   public:
                           AList(void) : Array(NULL), ArraySize(0), ItemCount(0) {};
      virtual              ~AList(void);

               int32_t     AddItem(T Item)
                           {
                              if(ItemCount >= ArraySize)
                              {
                                 uint32_t NewSize = ArraySize != 0 ? ArraySize*2 : 16;
                                 NewSize *= sizeof(T);
                                 T* ExtArray = (T*)realloc(Array, NewSize);
                                 if(ExtArray == NULL)
                                    return(-1);

                                 Array = ExtArray;
                                 ArraySize = NewSize;
                              }
                              if(Array == NULL)
                                 return(-1);

                              Array[ItemCount] = Item;
                              ItemCount++;
                              return(0);
                           };

               int32_t     DelItem(T Item)
                           {
                              uint32_t i;

                              for(i=0; i<ItemCount; i++)
                              {
                                 if(Item == Array[i])
                                 {
                                    if(i<ItemCount-1) // nothing to move on last item
                                       memmove(&Array[i], &Array[i+1], sizeof(T));
                                    ItemCount -=1 ;
                                    return(0);
                                 }
                              }
                              return(-1);
                           };

               int32_t     GetItemAt(uint32_t i_Index, T *p_Item) const
                           {
                              if( (p_Item == NULL) || (i_Index >= ItemCount) )
                                 return(-1);

                              *p_Item = Array[i_Index];
                              return(0);
                           };

               uint32_t    CountItems(void) const { return(ItemCount); };

      private:
               T           *Array;
               uint32_t    ArraySize;
               uint32_t    ItemCount;
};

#endif /* ALIST_H */
