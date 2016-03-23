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
* @date     2012/01/04
*
*****************************************************************************/
#include "CppUTest/TestHarness.h"
#include <string.h>
#include "OakString.h"

#define STRING1 "A test string"
#define STRING2 "A second test string"
#define STRING3 "test"

TEST_GROUP(StringTestGroup)
{
};

TEST(StringTestGroup, Basic)
{
   String AString;
   CHECK(AString.InitCheck() == 0);
   CHECK(AString.SetTo(STRING1) == 0);
   CHECK(strcmp(AString.GetString(), STRING1) == 0);

   String BString(AString);
   CHECK(BString.InitCheck() == 0);
   CHECK(strcmp(BString.GetString(), STRING1) == 0);

   CHECK(AString.SetTo(STRING2) == 0);
   CHECK(strcmp(AString.GetString(), STRING2) == 0);

   String CString;
   CHECK(CString.InitCheck() == 0);
   CHECK(CString.SetTo(BString) == 0);
   CHECK(strcmp(CString.GetString(), BString.GetString()) == 0);


};


TEST(StringTestGroup, Find)
{
   String AString;
   CHECK(AString.InitCheck() == 0);
   CHECK(AString.SetTo(STRING1) == 0);

   String BString;
   CHECK(BString.InitCheck() == 0);
   CHECK(BString.SetTo(STRING3) == 0);
   CHECK(AString.FindFirst(BString) == 2);

};

TEST(StringTestGroup, Operator1)
{
   String AString;
   CHECK(AString.InitCheck() == 0);
   CHECK(AString.SetTo(STRING1) == 0);

   String BString;
   CHECK(BString.InitCheck() == 0);
   CHECK(BString == "");
   CHECK(BString.SetTo(STRING2) == 0);

   CHECK(BString != AString);
   CHECK(BString.SetTo(STRING1) == 0);
   CHECK(BString == AString);
}


TEST(StringTestGroup, Operator2)
{
   String AString(STRING1);
   AString << STRING2;
   CHECK(AString == STRING1 STRING2);
   AString << 938;
   CHECK(AString == STRING1 STRING2 "938");
   String BString(STRING2);
   AString << BString;
   CHECK(AString == STRING1 STRING2 "938" STRING2);
   BString << BString;
   CHECK(BString == STRING2 STRING2);

   String CString;
   CString << "foo";
   CString << "foo2";
   CString << "foo3";
   uint32_t i;
   for(i=0; i< 4000; i++)
      CString << "foo4";
}
