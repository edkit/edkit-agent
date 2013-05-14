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
* @author   R. Picard
* @date     2012/01/04
*
*****************************************************************************/
#include "CppUTest/TestHarness.h"
#include "DynObject.h"

#define String1 "A test string"
#define STRING2 "A second test string"

TEST_GROUP(DynObjectTestGroup)
{
};

TEST(DynObjectTestGroup, Build)
{
   DynObject *Obj1 = new(std::nothrow)DynObject();

   {
      DynObject Obj2;
      DynObject Obj3(*Obj1);
   }
   delete Obj1;
}


TEST(DynObjectTestGroup, IntBasic)
{
   DynObject Obj1;

   /* Add items */
   CHECK(Obj1.InitCheck() == 0);
   CHECK(Obj1.AddInt("int1", 8) == 0);
   CHECK(Obj1.AddInt("int1", 5) == -1);
   CHECK(Obj1.AddInt("int5", 2325) == 0);
   CHECK(Obj1.AddInt("int15", 4025) == 0);

   /* Retrieve them */
   int64_t i;
   CHECK(Obj1.FindInt("int1", &i) == 0);
   CHECK(i == 8);
   CHECK(Obj1.FindInt("int2", &i) == -1);
   CHECK(Obj1.FindInt("int5", &i) == 0);
   CHECK(i == 2325);
   CHECK(Obj1.FindInt("int15", &i) == 0);
   CHECK(i == 4025);

   /* Check their existence */
   CHECK(Obj1.HasEntry("int1") == true);
   CHECK(Obj1.HasEntry("int5") == true);
   CHECK(Obj1.HasEntry("int15") == true);
   CHECK(Obj1.HasEntry("foo") == false);

   /* Introspection */
   DynEntry<int64_t> *Entry;
   Entry = dynamic_cast<DynEntry<int64_t>*>(Obj1.GetEntryAt(0));
   CHECK( Entry != NULL);
   CHECK( Entry->GetValue() == 8);
   Entry = dynamic_cast<DynEntry<int64_t>*>(Obj1.GetEntryAt(1));
   CHECK( Entry != NULL);
   CHECK( Entry->GetValue() == 2325);
   Entry = dynamic_cast<DynEntry<int64_t>*>(Obj1.GetEntryAt(2));
   CHECK( Entry != NULL);
   CHECK( Entry->GetValue() == 4025);

   /* Clean all */
   CHECK(Obj1.MakeEmpty() == 0);
   CHECK(Obj1.HasEntry("int1") == false);
   CHECK(Obj1.HasEntry("int5") == false);
   CHECK(Obj1.HasEntry("int15") == false);
};


TEST(DynObjectTestGroup, StringBasic)
{
   DynObject Obj1;

   /* Add items */
   CHECK(Obj1.InitCheck() == 0);
   CHECK(Obj1.AddString("str1", "string1") == 0);
   CHECK(Obj1.AddString("str1", "string2") == -1);
   CHECK(Obj1.AddString("str5", "string5") == 0);
   CHECK(Obj1.AddString("str15", "string15") == 0);

   /* Retrieve them */
   const String *p_String;
   CHECK(Obj1.FindString("str1", &p_String) == 0);
   CHECK(*p_String == "string1");
   CHECK(Obj1.FindString("str2", &p_String) == -1);
   CHECK(Obj1.FindString("str5", &p_String) == 0);
   CHECK(*p_String == "string5");
   CHECK(Obj1.FindString("str15", &p_String) == 0);
   CHECK(*p_String == "string15");

   /* Check their existence */
   CHECK(Obj1.HasEntry("str1") == true);
   CHECK(Obj1.HasEntry("str5") == true);
   CHECK(Obj1.HasEntry("str15") == true);
   CHECK(Obj1.HasEntry("foo") == false);

   /* Introspection */
   DynEntry<const String> *Entry;
   Entry = dynamic_cast<DynEntry<const String>*>(Obj1.GetEntryAt(0));
   CHECK( Entry != NULL);
   CHECK( Entry->GetValue() == "string1");
   CHECK( *(Entry->GetValuePtr()) == "string1");
   Entry = dynamic_cast<DynEntry<const String>*>(Obj1.GetEntryAt(1));
   CHECK( Entry != NULL);
   CHECK( Entry->GetValue() == "string5");
   CHECK( *(Entry->GetValuePtr()) == "string5");
   Entry = dynamic_cast<DynEntry<const String>*>(Obj1.GetEntryAt(2));
   CHECK( Entry != NULL);
   CHECK( Entry->GetValue() == "string15");
   CHECK( *(Entry->GetValuePtr()) == "string15");

   /* Clean all */
   CHECK(Obj1.MakeEmpty() == 0);
   CHECK(Obj1.HasEntry("str1") == false);
   CHECK(Obj1.HasEntry("str5") == false);
   CHECK(Obj1.HasEntry("str15") == false);

};


TEST(DynObjectTestGroup, IntCopy)
{
   DynObject Obj1;

   /* Add items */
   CHECK(Obj1.InitCheck() == 0);
   CHECK(Obj1.AddInt("int1", 8) == 0);
   CHECK(Obj1.AddInt("int1", 5) == -1);
   CHECK(Obj1.AddInt("int5", 2325) == 0);
   CHECK(Obj1.AddInt("int15", 4025) == 0);

   /* Copy object */
   DynObject Obj1Copy(Obj1);

   /* Retrieve them */
   int64_t i;
   CHECK(Obj1Copy.FindInt("int1", &i) == 0);
   CHECK(i == 8);
   CHECK(Obj1Copy.FindInt("int2", &i) == -1);
   CHECK(Obj1Copy.FindInt("int5", &i) == 0);
   CHECK(i == 2325);
   CHECK(Obj1Copy.FindInt("int15", &i) == 0);
   CHECK(i == 4025);
};

