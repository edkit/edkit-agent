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
#include <new>
#include "String.h"
#include "ListIterator.h"
#include "DynObject.h"

/**
* @date     2011/12/19
*
*  Constructor.
*
******************************************************************************/
DynObject::DynObject(void):
   InitStatus(0), EntryList()
{
}

/**
* @date     2011/12/19
*
*  copy Constructor.
*
******************************************************************************/
DynObject::DynObject(const DynObject &Object):
   InitStatus(-1), EntryList()
{
   DynEntryBase   *p_EntryBase;
   bool     b_End = false;
   bool     b_Handled = false;
   uint32_t i_Index = 0;
   DynEntry<int64_t> *p_IntEntry;
   DynEntry<const String> *p_StringEntry;
   DynEntry<const DynObject> *p_DynObjectEntry;

   while(b_End == false)
   {
      p_EntryBase = Object.GetEntryAt(i_Index);
      if(p_EntryBase == NULL)
      {
         b_Handled = true;
         b_End = true;
      }
      else
      {
         b_Handled = false;
         p_IntEntry = dynamic_cast<DynEntry<int64_t>*>(p_EntryBase);
         if(p_IntEntry != NULL)
         {
            AddInt(p_IntEntry->GetName(), p_IntEntry->GetValue());
            b_Handled = true;
         }

         p_StringEntry = dynamic_cast<DynEntry<const String>*>(p_EntryBase);
         if(p_StringEntry != NULL)
         {
            AddString(p_StringEntry->GetName(), *(p_StringEntry->GetValuePtr()));
            b_Handled = true;
         }

         p_DynObjectEntry = dynamic_cast<DynEntry<const DynObject>*>(p_EntryBase);
         if(p_DynObjectEntry != NULL)
         {
            AddDynObject(p_DynObjectEntry->GetName(), *(p_DynObjectEntry->GetValuePtr()));
            b_Handled = true;
         }

         // all entries must be added, otherwise there is a problem
         if(b_Handled == false)
            b_End = true;
         i_Index++;
      }
   }

   if(b_Handled == true)
      InitStatus = 0;
   return;
}


/**
* @date     2011/12/19
*
*  Destructor.
*
******************************************************************************/
DynObject::~DynObject(void)
{
   MakeEmpty();
}


/**
* @date     2011/12/19
*
*  Returns the entry stored at the provided index in the entries list. This API
*  is usefull for introspection.
*
******************************************************************************/
DynEntryBase* DynObject::GetEntryAt(uint32_t i_Index) const
{
   DynEntryBase   *p_DynEntryBase = NULL;

   if(EntryList.GetItemAt(i_Index, &p_DynEntryBase) == 0)
      return(p_DynEntryBase);
   return(NULL);
}


/**
* @date     2012/01/05
*
*  Tells whether the requested entry exists.
*
******************************************************************************/
bool DynObject::HasEntry(const char *Name) const
{
   ListIterator<AList<DynEntryBase*>, DynEntryBase*> Iterator;
   DynEntryBase *p_Entry;

   Iterator.SetTo(&EntryList);
   Iterator.First();
   while(Iterator.IsDone() == false)
   {
      if(Iterator.GetItem(&p_Entry) == 0)
      {
         if(strcmp(Name, p_Entry->GetName()) == 0)
            return(true);
      }
      Iterator.Next();
   }
   return(false);
}


/**
* @date     2012/01/05
*
*  Removes all entries from the object.
*
******************************************************************************/
int32_t DynObject::MakeEmpty(void)
{
   DynEntryBase *p_Entry;

   uint32_t i_ItemsCount = EntryList.CountItems();
   if(i_ItemsCount == 0)
      return(0);

   int i;
   for(i=i_ItemsCount-1; i>=0; i--)
   {
      if(EntryList.GetItemAt(i, &p_Entry) == 0)
      {
         EntryList.DelItemAt(i);
         delete p_Entry;
      }
   }
   return(0);

}


#define DEFINE_ADD_FUNCTION(type, typeName)                                \
int32_t DynObject::Add##typeName(const char *Name, type Value)             \
{                                                                          \
   int32_t i_Ret = -1;                                                     \
                                                                           \
   if(HasEntry(Name) == true)                                              \
      return(-1);                                                          \
                                                                           \
   DynEntry<type> *Entry = new(std::nothrow) DynEntry<type>(Name, Value);  \
   if(Entry != NULL)                                                       \
   {                                                                       \
      i_Ret = EntryList.AddItem(Entry);                                    \
      if(i_Ret != 0)                                                       \
         delete Entry;                                                     \
   }                                                                       \
   return(i_Ret);                                                          \
}


#define DEFINE_ADDREF_FUNCTION(type, typeName)                             \
int32_t DynObject::Add##typeName(const char *Name, type &Value)            \
{                                                                          \
   int32_t i_Ret = -1;                                                     \
                                                                           \
   if(HasEntry(Name) == true)                                              \
      return(-1);                                                          \
                                                                           \
   DynEntry<type> *Entry = new(std::nothrow) DynEntry<type>(Name, Value);  \
   if(Entry != NULL)                                                       \
   {                                                                       \
      i_Ret = EntryList.AddItem(Entry);                                    \
      if(i_Ret != 0)                                                       \
         delete Entry;                                                     \
   }                                                                       \
   return(i_Ret);                                                          \
}


#define DEFINE_FIND_FUNCTION(type, typeName)                               \
int32_t DynObject::Find##typeName(const char *Name, type *Value) const     \
{                                                                          \
   ListIterator<AList<DynEntryBase*>, DynEntryBase*> Iterator;             \
   DynEntry<type> *p_Item = NULL;                                          \
                                                                           \
   Iterator.SetTo(&EntryList);                                             \
   Iterator.First();                                                       \
   while(Iterator.IsDone() == false)                                       \
   {                                                                       \
      DynEntryBase *p_BaseItem = p_Item;                                   \
      if( (Iterator.GetItem(&p_BaseItem) == 0) &&                          \
            (p_BaseItem != NULL) )                                         \
      {                                                                    \
         p_Item = dynamic_cast<DynEntry<type>*>(p_BaseItem);               \
         if(p_Item != NULL)                                                \
         {                                                                 \
            if(strcmp(Name, p_Item->GetName()) == 0)                       \
            {                                                              \
               *Value = p_Item->GetValue();                                \
               return(0);                                                  \
            }                                                              \
         }                                                                 \
      }                                                                    \
      Iterator.Next();                                                     \
   }                                                                       \
   return(-1);                                                             \
}


#define DEFINE_FINDPTR_FUNCTION(type, typeName)                              \
int32_t DynObject::Find##typeName(const char *Name, type **Value) const    \
{                                                                          \
   ListIterator<AList<DynEntryBase*>, DynEntryBase*> Iterator;             \
   DynEntry<type> *p_Item = NULL;                                          \
                                                                           \
   Iterator.SetTo(&EntryList);                                             \
   Iterator.First();                                                       \
   while(Iterator.IsDone() == false)                                       \
   {                                                                       \
      DynEntryBase *p_BaseItem = p_Item;                                   \
      if( (Iterator.GetItem(&p_BaseItem) == 0) &&                          \
            (p_BaseItem != NULL) )                                         \
      {                                                                    \
         p_Item = dynamic_cast<DynEntry<type>*>(p_BaseItem);               \
         if(p_Item != NULL)                                                \
         {                                                                 \
            if(strcmp(Name, p_Item->GetName()) == 0)                       \
            {                                                              \
               *Value = p_Item->GetValuePtr();                             \
               return(0);                                                  \
            }                                                              \
         }                                                                 \
      }                                                                    \
      Iterator.Next();                                                     \
   }                                                                       \
   return(-1);                                                             \
}

DEFINE_ADD_FUNCTION(int64_t, Int)
DEFINE_FIND_FUNCTION(int64_t, Int)

DEFINE_ADDREF_FUNCTION(const String, String)
DEFINE_FINDPTR_FUNCTION(const String, String)

DEFINE_ADDREF_FUNCTION(const DynObject, DynObject)
DEFINE_FINDPTR_FUNCTION(const DynObject, DynObject)

