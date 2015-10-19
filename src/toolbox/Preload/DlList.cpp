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
#include "DlList.h"


/**
* @date     2011/05/19
*
*  Constructor.
*
******************************************************************************/
DlList::DlList(void):
   Head(NULL), Tail(NULL)
{
   return;
}


/**
* @date     2011/05/19
*
*  Destructor.
*
******************************************************************************/
DlList::~DlList(void)
{
   return;
}


/**
* @date     2011/05/19
*
*  Inserts an item at the start of the list. Once inserted, the item belongs to
*  the list and must not be deleted.
*
*  @param Item (in): Item to add.
*
******************************************************************************/
void DlList::PrependItem(DlListItem *Item)
{
   DlListItem *Next;

   if(Item == NULL)
      return;

   Item->Prev = NULL;
   if(Head == NULL)
   {
      Item->Next = NULL;
   }
   else
   {
      Next = Head;
      Item->Next = Next;
      Next->Prev = Item;
   }

   if(Tail == NULL)
      Tail = Item;
   Head = Item;
}


/**
* @date     2011/05/19
*
*  Inserts an item at the end of the list. Once inserted, the item belongs to
*  the list and must not be deleted.
*
*  @param Item (in): Item to add.
*
******************************************************************************/
void DlList::AppendItem(DlListItem *Item)
{
   DlListItem *Prev;

   if(Item == NULL)
      return;

   Item->Next = NULL;
   if(Tail == NULL)
   {
      Item->Prev = NULL;
   }
   else
   {
      Prev = Tail;
      Item->Prev = Prev;
      Prev->Next = Item;
   }

   if(Head == NULL)
      Head = Item;
   Tail = Item;
}


/**
* @date     2011/05/19
*
*  Removes an item from the list. The caller is responsible for deleting the
*  object removed from the list.
*
*  @param Item (in): Item to remove.
*
******************************************************************************/
void DlList::DelItem(DlListItem *Item)
{
   DlListItem *Prev, *Next;

   if(Item == NULL)
      return;

   Next = Item->Next;
   Prev = Item->Prev;
   if(Item == Head)
   {
      if(Next != NULL)
         Next->Prev = NULL;
      Head = Next;
      if(Item == Tail)
         Tail = Head;
   }
   else if(Item == Tail)
   {
      if(Prev != NULL)
         Prev->Next = NULL;
      Tail = Prev;
      if(Item == Head)
         Head = Tail;
   }
   else
   {
      Prev->Next = Next;
      Next->Prev = Prev;
   }
}

/**
* @date     2013/07/28
*
*  Removes all items from the list. The caller is responsible for deleting the
*  objects removed from the list.
*
******************************************************************************/
void DlList::MakeEmpty(void)
{
   while(Head != NULL)
   {
      DelItem(Head);
   }
}

