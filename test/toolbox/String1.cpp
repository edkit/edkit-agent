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
#include <string.h>
#include "String.h"
#include "String1.h"

CPPUNIT_TEST_SUITE_REGISTRATION( String1 );

#define STRING1 "A test string"
#define STRING2 "A second test string"
#define STRING3 "test"

void String1::setUp()
{
}


void String1::tearDown()
{
}

void String1::TestBasic()
{
   String AString;
   CPPUNIT_ASSERT(AString.InitCheck() == 0);
   CPPUNIT_ASSERT(AString.SetTo(STRING1) == 0);
   CPPUNIT_ASSERT(strcmp(AString.GetString(), STRING1) == 0);

   String BString(AString);
   CPPUNIT_ASSERT(BString.InitCheck() == 0);
   CPPUNIT_ASSERT(strcmp(BString.GetString(), STRING1) == 0);

   CPPUNIT_ASSERT(AString.SetTo(STRING2) == 0);
   CPPUNIT_ASSERT(strcmp(AString.GetString(), STRING2) == 0);

   String CString;
   CPPUNIT_ASSERT(CString.InitCheck() == 0);
   CPPUNIT_ASSERT(CString.SetTo(BString) == 0);
   CPPUNIT_ASSERT(strcmp(CString.GetString(), BString.GetString()) == 0);


};


void String1::TestFind()
{
   String AString;
   CPPUNIT_ASSERT(AString.InitCheck() == 0);
   CPPUNIT_ASSERT(AString.SetTo(STRING1) == 0);

   String BString;
   CPPUNIT_ASSERT(BString.InitCheck() == 0);
   CPPUNIT_ASSERT(BString.SetTo(STRING3) == 0);
   CPPUNIT_ASSERT(AString.FindFirst(BString) == 2);

};

void String1::TestOperator1()
{
   String AString;
   CPPUNIT_ASSERT(AString.InitCheck() == 0);
   CPPUNIT_ASSERT(AString.SetTo(STRING1) == 0);

   String BString;
   CPPUNIT_ASSERT(BString.InitCheck() == 0);
   CPPUNIT_ASSERT(BString == "");
   CPPUNIT_ASSERT(BString.SetTo(STRING2) == 0);

   CPPUNIT_ASSERT(BString != AString);
   CPPUNIT_ASSERT(BString.SetTo(STRING1) == 0);
   CPPUNIT_ASSERT(BString == AString);
}


void String1::TestOperator2()
{
   String AString(STRING1);
   AString << STRING2;
   CPPUNIT_ASSERT(AString == STRING1 STRING2);
   AString << 938;
   CPPUNIT_ASSERT(AString == STRING1 STRING2 "938");
   String BString(STRING2);
   AString << BString;
   CPPUNIT_ASSERT(AString == STRING1 STRING2 "938" STRING2);
   BString << BString;
   CPPUNIT_ASSERT(BString == STRING2 STRING2);

   String CString;
   CString << "foo";
   CString << "foo2";
   CString << "foo3";
   uint32_t i;
   for(i=0; i< 4000; i++)
      CString << "foo4";
}
