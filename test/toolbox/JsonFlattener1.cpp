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
#include "CppUTest/TestHarness.h"
#include "JsonFlattener.h"


#define STRING1 "A test string"
#define STRING2 "A second test string"
#define STRING3 "A Third test string"

TEST_GROUP(JsonFlattenerTestGroup)
{
};

TEST(JsonFlattenerTestGroup, Build)
{
   JsonFlattener *Obj1 = new JsonFlattener();

   {
      JsonFlattener Obj2;
   }
   delete Obj1;
}

TEST(JsonFlattenerTestGroup, UnflattenInt)
{
   DynObject      Obj1;
   JsonFlattener  Flattener1;
   String         Json("{ foo: 32, bar:42, other:892382}");

   CHECK(Json.InitCheck() == 0);
   CHECK(Flattener1.UnFlatten(&Json, &Obj1) == -1);

   CHECK(Json.SetTo("{ \"foo\": 32, \"bar\":42, \"other\":892382}") == 0);
   CHECK(Json.InitCheck() == 0);
   CHECK(Flattener1.UnFlatten(&Json, &Obj1) == 0);

   int64_t i;
   CHECK(Obj1.FindInt("foo", &i) == 0);
   CHECK(i == 32);
   CHECK(Obj1.FindInt("bar", &i) == 0);
   CHECK(i == 42);
   CHECK(Obj1.FindInt("other", &i) == 0);
   CHECK(i == 892382);

};


TEST(JsonFlattenerTestGroup, UnflattenString)
{
   DynObject      Obj1;
   JsonFlattener  Flattener1;
   String         Json("{ \"foo\": \"" STRING1 "\", \"bar\":\"" STRING2 "\", \"other\":    \"" STRING3 "\"}");

   CHECK(Json.InitCheck() == 0);
   CHECK(Flattener1.UnFlatten(&Json, &Obj1) == 0);

   const String *s;
   CHECK(Obj1.FindString("foo", &s) == 0);
   CHECK(s != NULL);
   CHECK(*s == STRING1);
   CHECK(Obj1.FindString("bar", &s) == 0);
   CHECK(s != NULL);
   CHECK(*s == STRING2);
   CHECK(Obj1.FindString("other", &s) == 0);
   CHECK(s != NULL);
   CHECK(*s == STRING3);
};

TEST(JsonFlattenerTestGroup, UnflattenObj)
{
   DynObject         Obj1;
   const DynObject   *p_Obj2;
   JsonFlattener  Flattener1;
#define OBJ2   "{ \"foo\": \"" STRING1 "\", \"bar\":\"" STRING2 "\", \"other\":    \"" STRING3 "\"}"
   String         Json("{ \"obj\": " OBJ2 "}");

   CHECK(Json.InitCheck() == 0);
   CHECK(Flattener1.UnFlatten(&Json, &Obj1) == 0);
   CHECK(Obj1.FindDynObject("obj", &p_Obj2) == 0);
   CHECK(p_Obj2 != NULL);

   const String *s;
   CHECK(p_Obj2->FindString("foo", &s) == 0);
   CHECK(s != NULL);
   CHECK(*s == STRING1);
   CHECK(p_Obj2->FindString("bar", &s) == 0);
   CHECK(s != NULL);
   CHECK(*s == STRING2);
   CHECK(p_Obj2->FindString("other", &s) == 0);
   CHECK(s != NULL);
   CHECK(*s == STRING3);
};


TEST(JsonFlattenerTestGroup, FlattenInt)
{
#define FLATTEN_INT_RESULT "{ \"foo\": 32, \"bar\": 42, \"other\": 892382 }"

   DynObject      Obj1;
   JsonFlattener  Flattener1;
   String         Json;

   CHECK(Obj1.AddInt("foo", 32) == 0);
   CHECK(Obj1.AddInt("bar", 42) == 0);
   CHECK(Obj1.AddInt("other", 892382) == 0);

   CHECK(Flattener1.Flatten(&Obj1, &Json) == 0);
   CHECK(Json == FLATTEN_INT_RESULT);
};


TEST(JsonFlattenerTestGroup, FlattenString)
{
#define FLATTEN_STRING_RESULT "{ \"foo\": \"" STRING1 "\", \"bar\": \"" STRING2 "\", \"other\": \"" STRING3 "\" }"
   DynObject      Obj1;
   JsonFlattener  Flattener1;
   String         Json;

   CHECK(Obj1.AddString("foo", STRING1) == 0);
   CHECK(Obj1.AddString("bar", STRING2) == 0);
   CHECK(Obj1.AddString("other", STRING3) == 0);

   CHECK(Flattener1.Flatten(&Obj1, &Json) == 0);
   CHECK(Json == FLATTEN_STRING_RESULT);

};

TEST(JsonFlattenerTestGroup, FlattenObj)
{
#define FLATTEN_OBJ2   "{ \"foo\": \"" STRING1 "\", \"bar\": \"" STRING2 "\", \"other\": \"" STRING3 "\" }"
#define FLATTEN_OBJ_RESULT "{ \"obj\": " FLATTEN_OBJ2 " }"

   DynObject      Obj1, Obj2;
   JsonFlattener  Flattener1;
   String         Json;

   CHECK(Obj2.AddString("foo", STRING1) == 0);
   CHECK(Obj2.AddString("bar", STRING2) == 0);
   CHECK(Obj2.AddString("other", STRING3) == 0);
   CHECK(Obj1.AddDynObject("obj", Obj2) == 0);

   CHECK(Flattener1.Flatten(&Obj1, &Json) == 0);
   CHECK(Json == FLATTEN_OBJ_RESULT);
};

