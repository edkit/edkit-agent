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
* coEntryes of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all coEntryes or substantial portions of the Software.
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
* @author   R. Entrycard
* @date     2012/01/04
*
*****************************************************************************/
#include "DynObject.h"
#include "DynObject1.h"

CPPUNIT_TEST_SUITE_REGISTRATION( DynObject1 );

#define String1 "A test string"
#define STRING2 "A second test string"

void DynObject1::setUp()
{
}


void DynObject1::tearDown()
{
}

void DynObject1::TestBuild()
{
   DynObject *Obj1 = new(std::nothrow)DynObject();

   {
      DynObject Obj2;
      DynObject Obj3(*Obj1);
   }
   delete Obj1;
}


void DynObject1::TestIntBasic()
{
   DynObject Obj1;

   /* Add items */
   CPPUNIT_ASSERT(Obj1.InitCheck() == 0);
   CPPUNIT_ASSERT(Obj1.AddInt("int1", 8) == 0);
   CPPUNIT_ASSERT(Obj1.AddInt("int1", 5) == -1);
   CPPUNIT_ASSERT(Obj1.AddInt("int5", 2325) == 0);
   CPPUNIT_ASSERT(Obj1.AddInt("int15", 4025) == 0);

   /* Retrieve them */
   int64_t i;
   CPPUNIT_ASSERT(Obj1.FindInt("int1", &i) == 0);
   CPPUNIT_ASSERT(i == 8);
   CPPUNIT_ASSERT(Obj1.FindInt("int2", &i) == -1);
   CPPUNIT_ASSERT(Obj1.FindInt("int5", &i) == 0);
   CPPUNIT_ASSERT(i == 2325);
   CPPUNIT_ASSERT(Obj1.FindInt("int15", &i) == 0);
   CPPUNIT_ASSERT(i == 4025);

   /* Check their existence */
   CPPUNIT_ASSERT(Obj1.HasEntry("int1") == true);
   CPPUNIT_ASSERT(Obj1.HasEntry("int5") == true);
   CPPUNIT_ASSERT(Obj1.HasEntry("int15") == true);
   CPPUNIT_ASSERT(Obj1.HasEntry("foo") == false);

   /* Introspection */
   DynEntry<int64_t> *Entry;
   Entry = dynamic_cast<DynEntry<int64_t>*>(Obj1.GetEntryAt(0));
   CPPUNIT_ASSERT( Entry != NULL);
   CPPUNIT_ASSERT( Entry->GetValue() == 8);
   Entry = dynamic_cast<DynEntry<int64_t>*>(Obj1.GetEntryAt(1));
   CPPUNIT_ASSERT( Entry != NULL);
   CPPUNIT_ASSERT( Entry->GetValue() == 2325);
   Entry = dynamic_cast<DynEntry<int64_t>*>(Obj1.GetEntryAt(2));
   CPPUNIT_ASSERT( Entry != NULL);
   CPPUNIT_ASSERT( Entry->GetValue() == 4025);

   /* Clean all */
   CPPUNIT_ASSERT(Obj1.MakeEmpty() == 0);
   CPPUNIT_ASSERT(Obj1.HasEntry("int1") == false);
   CPPUNIT_ASSERT(Obj1.HasEntry("int5") == false);
   CPPUNIT_ASSERT(Obj1.HasEntry("int15") == false);
};


void DynObject1::TestStringBasic()
{
   DynObject Obj1;

   /* Add items */
   CPPUNIT_ASSERT(Obj1.InitCheck() == 0);
   CPPUNIT_ASSERT(Obj1.AddString("str1", "string1") == 0);
   CPPUNIT_ASSERT(Obj1.AddString("str1", "string2") == -1);
   CPPUNIT_ASSERT(Obj1.AddString("str5", "string5") == 0);
   CPPUNIT_ASSERT(Obj1.AddString("str15", "string15") == 0);

   /* Retrieve them */
   const String *p_String;
   CPPUNIT_ASSERT(Obj1.FindString("str1", &p_String) == 0);
   CPPUNIT_ASSERT(*p_String == "string1");
   CPPUNIT_ASSERT(Obj1.FindString("str2", &p_String) == -1);
   CPPUNIT_ASSERT(Obj1.FindString("str5", &p_String) == 0);
   CPPUNIT_ASSERT(*p_String == "string5");
   CPPUNIT_ASSERT(Obj1.FindString("str15", &p_String) == 0);
   CPPUNIT_ASSERT(*p_String == "string15");

   /* Check their existence */
   CPPUNIT_ASSERT(Obj1.HasEntry("str1") == true);
   CPPUNIT_ASSERT(Obj1.HasEntry("str5") == true);
   CPPUNIT_ASSERT(Obj1.HasEntry("str15") == true);
   CPPUNIT_ASSERT(Obj1.HasEntry("foo") == false);

   /* Introspection */
   DynEntry<const String> *Entry;
   Entry = dynamic_cast<DynEntry<const String>*>(Obj1.GetEntryAt(0));
   CPPUNIT_ASSERT( Entry != NULL);
   CPPUNIT_ASSERT( Entry->GetValue() == "string1");
   CPPUNIT_ASSERT( *(Entry->GetValuePtr()) == "string1");
   Entry = dynamic_cast<DynEntry<const String>*>(Obj1.GetEntryAt(1));
   CPPUNIT_ASSERT( Entry != NULL);
   CPPUNIT_ASSERT( Entry->GetValue() == "string5");
   CPPUNIT_ASSERT( *(Entry->GetValuePtr()) == "string5");
   Entry = dynamic_cast<DynEntry<const String>*>(Obj1.GetEntryAt(2));
   CPPUNIT_ASSERT( Entry != NULL);
   CPPUNIT_ASSERT( Entry->GetValue() == "string15");
   CPPUNIT_ASSERT( *(Entry->GetValuePtr()) == "string15");

   /* Clean all */
   CPPUNIT_ASSERT(Obj1.MakeEmpty() == 0);
   CPPUNIT_ASSERT(Obj1.HasEntry("str1") == false);
   CPPUNIT_ASSERT(Obj1.HasEntry("str5") == false);
   CPPUNIT_ASSERT(Obj1.HasEntry("str15") == false);

};


void DynObject1::TestIntCopy()
{
   DynObject Obj1;

   /* Add items */
   CPPUNIT_ASSERT(Obj1.InitCheck() == 0);
   CPPUNIT_ASSERT(Obj1.AddInt("int1", 8) == 0);
   CPPUNIT_ASSERT(Obj1.AddInt("int1", 5) == -1);
   CPPUNIT_ASSERT(Obj1.AddInt("int5", 2325) == 0);
   CPPUNIT_ASSERT(Obj1.AddInt("int15", 4025) == 0);

   /* Copy object */
   DynObject Obj1Copy(Obj1);

   /* Retrieve them */
   int64_t i;
   CPPUNIT_ASSERT(Obj1Copy.FindInt("int1", &i) == 0);
   CPPUNIT_ASSERT(i == 8);
   CPPUNIT_ASSERT(Obj1Copy.FindInt("int2", &i) == -1);
   CPPUNIT_ASSERT(Obj1Copy.FindInt("int5", &i) == 0);
   CPPUNIT_ASSERT(i == 2325);
   CPPUNIT_ASSERT(Obj1Copy.FindInt("int15", &i) == 0);
   CPPUNIT_ASSERT(i == 4025);
};

