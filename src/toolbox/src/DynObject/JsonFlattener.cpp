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
* @date     2012/01/09
*
*****************************************************************************/
extern "C"
{
#include "json.h"
}
#include "JsonFlattener.h"
#include "String.h"


/**
* @date     2012/01/09
*
*  Constructor.
*
******************************************************************************/
JsonFlattener::JsonFlattener(void)
{
   return;
}


/**
* @date     2012/01/08
*
*  Destructor.
*
******************************************************************************/
JsonFlattener::~JsonFlattener(void)
{
   return;
}


/**
* @date     2012/01/08
*
*  Unflattens a json string to an object.
*
******************************************************************************/
int32_t JsonFlattener::UnFlatten(const String *p_Json, DynObject *p_Object)
{
   int32_t i_Ret = -1;
   struct json_object *p_JsonObject;

   if( (p_Json == NULL) || (p_Object == NULL) )
      return(-1);

   p_Object->MakeEmpty();
   p_JsonObject = json_tokener_parse(p_Json->GetString());
   if(is_error(p_JsonObject) == false)
   {
      i_Ret = UnFlatten(p_JsonObject, p_Object);
      json_object_put(p_JsonObject);
   }

   return(i_Ret);
}


/**
* @date     2012/01/22
*
*  Flattens an object to a json string.
*
******************************************************************************/
int32_t JsonFlattener::Flatten(const DynObject *p_Object, String *p_Json)
{
   int32_t i_Ret = -1;
   struct json_object *p_JsonObject;

   if( (p_Json == NULL) || (p_Object == NULL) )
      return(-1);

   p_Json->SetTo("");
   p_JsonObject = json_object_new_object();
   i_Ret = Flatten(p_Object, p_JsonObject);
   if(i_Ret == 0)
   {
      p_Json->SetTo(json_object_to_json_string(p_JsonObject));
   }

   json_object_put(p_JsonObject);
   return(i_Ret);
}


/**
* @date     2012/01/08
*
*  Unflattens a json object to an object. This method is recursive.
*
******************************************************************************/
int32_t JsonFlattener::UnFlatten(struct json_object *p_JsonObject, DynObject *p_Object)
{
   int32_t     i_Ret = 0;
   DynObject   ChildObject;

   if( (p_JsonObject == NULL) || (p_Object == NULL) )
      return(-1);

   json_object_object_foreach(p_JsonObject, Key, Val)
   {
      switch(json_object_get_type(Val))
      {
         case json_type_int:
            i_Ret = p_Object->AddInt(Key, json_object_get_int(Val));
            break;

         case json_type_object:
            i_Ret = UnFlatten(Val, &ChildObject);
            if(i_Ret == 0)
            {
               i_Ret = p_Object->AddDynObject(Key, ChildObject);
            }
            ChildObject.MakeEmpty();
            break;

         case json_type_string:
            i_Ret = p_Object->AddString(Key, String(json_object_get_string(Val)));
            break;

         case json_type_array:
         case json_type_boolean:
         case json_type_double:
         default:
            i_Ret = -1;
            break;
      }
      if(i_Ret != 0)
         break;
   }

   return(i_Ret);
}



/**
* @date     2012/01/22
*
*  Flattens a dynamic object to a json object. This method is recursive.
*
******************************************************************************/
int32_t JsonFlattener::Flatten(const DynObject *p_Object, struct json_object *p_JsonObject)
{
   int32_t     i_Ret = 0;
   DynObject   ChildObject;

   if( (p_JsonObject == NULL) || (p_Object == NULL) )
      return(-1);

   bool     b_End = false;
   bool     b_Handled = false;
   uint32_t i_Index = 0;
   DynEntryBase            *p_EntryBase;
   DynEntry<int64_t>       *p_IntEntry;
   DynEntry<const String>  *p_StringEntry;
   DynEntry<const DynObject> *p_DynObjectEntry;

   while(b_End == false)
   {
      p_EntryBase = p_Object->GetEntryAt(i_Index);
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
            json_object_object_add(p_JsonObject,
                  p_IntEntry->GetName(),
                  json_object_new_int(p_IntEntry->GetValue()));
            b_Handled = true;
         }

         p_StringEntry = dynamic_cast<DynEntry<const String>*>(p_EntryBase);
         if(p_StringEntry != NULL)
         {
            json_object_object_add(p_JsonObject,
                  p_StringEntry->GetName(),
                  json_object_new_string(p_StringEntry->GetValuePtr()->GetString()));
            b_Handled = true;
         }

         p_DynObjectEntry = dynamic_cast<DynEntry<const DynObject>*>(p_EntryBase);
         if(p_DynObjectEntry != NULL)
         {
            struct json_object *p_ChildJson = json_object_new_object();

            i_Ret = Flatten(p_DynObjectEntry->GetValuePtr(), p_ChildJson);
            if(i_Ret == 0)
            {
               json_object_object_add(p_JsonObject,
                     p_DynObjectEntry->GetName(),
                     p_ChildJson);
               b_Handled = true;
            }
            else
            {
               json_object_put(p_ChildJson);
            }
         }

         // all entries must be added, otherwise there is a problem
         if(b_Handled == false)
            b_End = true;
         i_Index++;
      }
   }

   if(b_Handled == false)
      return(-1);
   return(0);
}

