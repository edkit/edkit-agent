#ifndef DLLIST_H
#define DLLIST_H
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
* @date     2011/05/19
*
*****************************************************************************/
#include "stdlib.h"

struct DlListItem
{
      DlListItem(void): Prev(NULL), Next(NULL) {} ;
      virtual  ~DlListItem(void) {};

      DlListItem *Prev;
      DlListItem *Next;
};


class DlList
{
   public:
      DlList(void);
      virtual  ~DlList(void);

      DlListItem* GetHead(void) { return(Head); };
      DlListItem* GetTail(void) { return(Tail); };

      void PrependItem(DlListItem *Item);
      void AppendItem(DlListItem *Item);
      void DelItem(DlListItem *Item);
      void MakeEmpty(void);

   private:
      DlListItem *Head;
      DlListItem *Tail;
};


#endif /* DLLIST_H */
