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
#include "JsonFlattener.h"
#include "JsonFlattener1.h"

CPPUNIT_TEST_SUITE_REGISTRATION( JsonFlattener1 );

#define STRING1 "A test string"
#define STRING2 "A second test string"
#define STRING3 "A Third test string"

void JsonFlattener1::setUp()
{
}


void JsonFlattener1::tearDown()
{
}

void JsonFlattener1::TestBuild()
{
   JsonFlattener *Obj1 = new(std::nothrow)JsonFlattener();

   {
      JsonFlattener Obj2;
   }
   delete Obj1;
}

void JsonFlattener1::TestUnflattenInt()
{
   DynObject      Obj1;
   JsonFlattener  Flattener1;
   String         Json("{ foo: 32, bar:42, other:892382}");

   CPPUNIT_ASSERT(Json.InitCheck() == 0);
   CPPUNIT_ASSERT(Flattener1.UnFlatten(&Json, &Obj1) == -1);

   CPPUNIT_ASSERT(Json.SetTo("{ \"foo\": 32, \"bar\":42, \"other\":892382}") == 0);
   CPPUNIT_ASSERT(Json.InitCheck() == 0);
   CPPUNIT_ASSERT(Flattener1.UnFlatten(&Json, &Obj1) == 0);

   int64_t i;
   CPPUNIT_ASSERT(Obj1.FindInt("foo", &i) == 0);
   CPPUNIT_ASSERT(i == 32);
   CPPUNIT_ASSERT(Obj1.FindInt("bar", &i) == 0);
   CPPUNIT_ASSERT(i == 42);
   CPPUNIT_ASSERT(Obj1.FindInt("other", &i) == 0);
   CPPUNIT_ASSERT(i == 892382);

};


void JsonFlattener1::TestUnflattenString()
{
   DynObject      Obj1;
   JsonFlattener  Flattener1;
   String         Json("{ \"foo\": \"" STRING1 "\", \"bar\":\"" STRING2 "\", \"other\":    \"" STRING3 "\"}");

   CPPUNIT_ASSERT(Json.InitCheck() == 0);
   CPPUNIT_ASSERT(Flattener1.UnFlatten(&Json, &Obj1) == 0);

   const String *s;
   CPPUNIT_ASSERT(Obj1.FindString("foo", &s) == 0);
   CPPUNIT_ASSERT(s != NULL);
   CPPUNIT_ASSERT(*s == STRING1);
   CPPUNIT_ASSERT(Obj1.FindString("bar", &s) == 0);
   CPPUNIT_ASSERT(s != NULL);
   CPPUNIT_ASSERT(*s == STRING2);
   CPPUNIT_ASSERT(Obj1.FindString("other", &s) == 0);
   CPPUNIT_ASSERT(s != NULL);
   CPPUNIT_ASSERT(*s == STRING3);
};

void JsonFlattener1::TestUnflattenObj()
{
   DynObject         Obj1;
   const DynObject   *p_Obj2;
   JsonFlattener  Flattener1;
#define OBJ2   "{ \"foo\": \"" STRING1 "\", \"bar\":\"" STRING2 "\", \"other\":    \"" STRING3 "\"}"
   String         Json("{ \"obj\": " OBJ2 "}");

   CPPUNIT_ASSERT(Json.InitCheck() == 0);
   CPPUNIT_ASSERT(Flattener1.UnFlatten(&Json, &Obj1) == 0);
   CPPUNIT_ASSERT(Obj1.FindDynObject("obj", &p_Obj2) == 0);
   CPPUNIT_ASSERT(p_Obj2 != NULL);

   const String *s;
   CPPUNIT_ASSERT(p_Obj2->FindString("foo", &s) == 0);
   CPPUNIT_ASSERT(s != NULL);
   CPPUNIT_ASSERT(*s == STRING1);
   CPPUNIT_ASSERT(p_Obj2->FindString("bar", &s) == 0);
   CPPUNIT_ASSERT(s != NULL);
   CPPUNIT_ASSERT(*s == STRING2);
   CPPUNIT_ASSERT(p_Obj2->FindString("other", &s) == 0);
   CPPUNIT_ASSERT(s != NULL);
   CPPUNIT_ASSERT(*s == STRING3);
};


void JsonFlattener1::TestFlattenInt()
{
#define FLATTEN_INT_RESULT "{ \"foo\": 32, \"bar\": 42, \"other\": 892382 }"

   DynObject      Obj1;
   JsonFlattener  Flattener1;
   String         Json;

   CPPUNIT_ASSERT(Obj1.AddInt("foo", 32) == 0);
   CPPUNIT_ASSERT(Obj1.AddInt("bar", 42) == 0);
   CPPUNIT_ASSERT(Obj1.AddInt("other", 892382) == 0);

   CPPUNIT_ASSERT(Flattener1.Flatten(&Obj1, &Json) == 0);
   CPPUNIT_ASSERT(Json == FLATTEN_INT_RESULT);
};


void JsonFlattener1::TestFlattenString()
{
#define FLATTEN_STRING_RESULT "{ \"foo\": \"" STRING1 "\", \"bar\": \"" STRING2 "\", \"other\": \"" STRING3 "\" }"
   DynObject      Obj1;
   JsonFlattener  Flattener1;
   String         Json;

   CPPUNIT_ASSERT(Obj1.AddString("foo", STRING1) == 0);
   CPPUNIT_ASSERT(Obj1.AddString("bar", STRING2) == 0);
   CPPUNIT_ASSERT(Obj1.AddString("other", STRING3) == 0);

   CPPUNIT_ASSERT(Flattener1.Flatten(&Obj1, &Json) == 0);
   CPPUNIT_ASSERT(Json == FLATTEN_STRING_RESULT);

};

void JsonFlattener1::TestFlattenObj()
{
#define FLATTEN_OBJ2   "{ \"foo\": \"" STRING1 "\", \"bar\": \"" STRING2 "\", \"other\": \"" STRING3 "\" }"
#define FLATTEN_OBJ_RESULT "{ \"obj\": " FLATTEN_OBJ2 " }"

   DynObject      Obj1, Obj2;
   JsonFlattener  Flattener1;
   String         Json;

   CPPUNIT_ASSERT(Obj2.AddString("foo", STRING1) == 0);
   CPPUNIT_ASSERT(Obj2.AddString("bar", STRING2) == 0);
   CPPUNIT_ASSERT(Obj2.AddString("other", STRING3) == 0);
   CPPUNIT_ASSERT(Obj1.AddDynObject("obj", Obj2) == 0);

   CPPUNIT_ASSERT(Flattener1.Flatten(&Obj1, &Json) == 0);
   CPPUNIT_ASSERT(Json == FLATTEN_OBJ_RESULT);
};

