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
#include <stdio.h>
#include <string.h>

#include "String.h"

/**
* @date     2011/12/19
*
*  Constructor.
*
******************************************************************************/
String::String(void):
   InitStatus(0), Area(NULL), AreaSize(0)
{
}


/**
* @date     2011/12/21
*
*  Copy Constructor.
*
******************************************************************************/
String::String(const String &BaseString):
   InitStatus(-1), Area(NULL), AreaSize(0)
{
   InitStatus = SetTo(BaseString);
}


/**
* @date     2012/01/11
*
*  Constructor.
*
******************************************************************************/
String::String(const char *BaseString, uint32_t i_Length):
   InitStatus(-1), Area(NULL), AreaSize(0)
{
   InitStatus = SetTo(BaseString, i_Length);
}


/**
* @date     2011/12/19
*
*  Destructor.
*
******************************************************************************/
String::~String(void)
{
   DelArea();
}


/**
* @date     2012/01/08
*
*  Equality operator.
*
******************************************************************************/
bool String::operator==(const String &Rhs) const
{
   if(strcmp(GetString(), Rhs.GetString()) == 0)
      return(true);
   return(false);
}

/**
* @date     2012/01/08
*
*  Equality operator.
*
******************************************************************************/
bool String::operator!=(const String &Rhs) const
{
   return(!operator==(Rhs));
}


/**
* @date     2012/02/14
*
*  Append operator.
*
******************************************************************************/
String& String::operator<<(int Rhs)
{
   char Num[32];
   int32_t i_Length = snprintf(Num, sizeof(Num), "%d", Rhs);

   AppendString(Num, i_Length);
   return(*this);
}


/**
* @date     2012/02/14
*
*  Append operator.
*
******************************************************************************/
String& String::operator<<(const char* Rhs)
{
   AppendString(Rhs, strlen(Rhs));
   return(*this);
}


/**
* @date     2012/02/14
*
*  Append operator.
*
******************************************************************************/
String& String::operator<<(const String &Rhs)
{
   AppendString(Rhs.GetString(), Rhs.GetSize());
   return(*this);
}


/**
* @date     2012/01/08
*
*  Sets the value of the string.
*
******************************************************************************/
int32_t String::SetTo(const String &Value)
{
   return(SetTo(Value.GetString(), 0));
}


/**
* @date     2011/12/19
*
*  Sets the value of the string.
*
******************************************************************************/
int32_t String::SetTo(const char *Value, uint32_t i_Length)
{
   if(Value == NULL)
      return(-1);

   DelArea();
   uint32_t ValueLength;
   if(i_Length == 0)
      ValueLength = strlen(Value);
   else
      ValueLength = i_Length;
   Area = static_cast<char*>(malloc(ValueLength+1));
   if(Area == NULL)
      return(-1);
   memcpy(Area, Value, strlen(Value));
   Area[ValueLength] = '\0';
   return(0);
}


/**
* @date     2011/12/21
*
*  Search SearchString inside the string object.
*
*  @param   SearchString (in): String to search for.
*  @return  SearchString offset is success.
*  @return  -1 otherwise.
******************************************************************************/
int32_t String::FindFirst(const String &SearchString) const
{
   const char *SearchCString = SearchString.GetString();
   char  *Result;

   if(SearchCString != NULL)
   {
      Result = strstr(Area, SearchCString);
      if(Result != NULL)
      {
         return(Result-Area);
      }
   }
   return(-1);
}


/**
* @date     2011/12/19
*
*  Frees the Area storage space.
*
******************************************************************************/
void String::DelArea(void)
{
   if(Area != NULL)
   {
      free(Area);
      Area = NULL;
   }
}


/**
* @date     2012/02/14
*
*  Appends a string.
* ///@todo : prealloc space to avoid multiple reallocs
*
******************************************************************************/
int32_t String::AppendString(const char* sz_String, uint32_t i_Length)
{
   if( (sz_String == NULL) || (i_Length == 0) )
      return(-1);

   bool     b_Duplicate = sz_String == Area ? true : false;
   uint32_t i_Offset = 0;
   if(Area != NULL)
      i_Offset = strlen(Area);

   uint32_t i_NewSize = strlen(Area)+i_Length+1;
   if(i_NewSize > AreaSize)
   {
      Area = static_cast<char*>(realloc(Area, i_NewSize));
      if(Area != NULL)
         AreaSize = i_NewSize;
   }
   if(Area == NULL)
      return(-1);
   if(b_Duplicate == true)
      memcpy(Area+i_Offset, Area, i_Length);
   else
      memcpy(Area+i_Offset, sz_String, i_Length);
   Area[i_Offset+i_Length] = '\0';
   return(0);
}
