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
#include "HttpdRequest.h"
#include "HttpdRequest1.h"
#include "ListIterator.h"

CPPUNIT_TEST_SUITE_REGISTRATION( HttpdRequest1 );


#define PATH1 "/ws/somepath/234"

#define HEADER1 "Header1"
#define HEADER2 "Header2"

#define HEADER1VAL   "Header1 Value"
#define HEADER2VAL   "Header2 Value"

void HttpdRequest1::setUp()
{
}


void HttpdRequest1::tearDown()
{
}

void HttpdRequest1::TestBuild()
{
   HttpdRequest *Obj1 = new(std::nothrow) HttpdRequest();

   {
      HttpdRequest Obj2;
   }
   delete Obj1;
}


void HttpdRequest1::TestBasic()
{
   String   SetPath(PATH1 );
   HttpdRequest Obj1;

   /* Path */
   const String   &GetPath1 = Obj1.GetPath();
   CPPUNIT_ASSERT(GetPath1 == "");
   CPPUNIT_ASSERT(Obj1.SetPath(SetPath) == 0);

   const String   &GetPath2 = Obj1.GetPath();
   CPPUNIT_ASSERT(GetPath2 == SetPath);

   /* Status */
   CPPUNIT_ASSERT(Obj1.GetStatus() == 200);
   Obj1.SetStatus(404);
   CPPUNIT_ASSERT(Obj1.GetStatus() == 404);

   /* Method */
   CPPUNIT_ASSERT(Obj1.GetMethod() == HttpdRequest::NONE);
   Obj1.SetMethod(HttpdRequest::POST);
   CPPUNIT_ASSERT(Obj1.GetMethod() == HttpdRequest::POST);

   /* payload */
   uint8_t SetPayload[326];
   const uint8_t *GetPayload;
   uint32_t i_Length;
   CPPUNIT_ASSERT(Obj1.SetPayload(SetPayload, 326) == 0);
   CPPUNIT_ASSERT(Obj1.GetPayload(&GetPayload, &i_Length) == 0);
   CPPUNIT_ASSERT(i_Length == 326);
   CPPUNIT_ASSERT(memcmp(GetPayload, SetPayload, i_Length) == 0);


   /* Header */
   String Header1Name(HEADER1), Header2Name(HEADER2);
   String Header1Val(HEADER1VAL), Header2Val(HEADER2VAL);
   String HeaderVal;

   CPPUNIT_ASSERT(Obj1.GetHeader(Header1Name, &HeaderVal) == -1);
   CPPUNIT_ASSERT(Obj1.SetHeader(Header1Name, Header1Val) == 0);
   CPPUNIT_ASSERT(Obj1.GetHeader(Header1Name, &HeaderVal) == 0);
   CPPUNIT_ASSERT(HeaderVal == Header1Val);

   CPPUNIT_ASSERT(Obj1.SetHeader(Header2Name, Header2Val) == 0);
   CPPUNIT_ASSERT(Obj1.GetHeader(Header2Name, &HeaderVal) == 0);
   CPPUNIT_ASSERT(HeaderVal == Header2Val);

   CPPUNIT_ASSERT(Obj1.SetHeader(Header1Name, Header2Val) == 0);
   CPPUNIT_ASSERT(Obj1.GetHeader(Header1Name, &HeaderVal) == 0);
   CPPUNIT_ASSERT(HeaderVal == Header2Val);

   /* Header list */
   const AList<HttpdRequest::Header*> &HeaderList = Obj1.GetHeaderList();
   ListIterator<AList<HttpdRequest::Header*>, HttpdRequest::Header*> Iterator;
   HttpdRequest::Header  *p_CurHeader;
   uint32_t i_Index = 0;

   Iterator.SetTo(&HeaderList);
   Iterator.First();
   while(Iterator.IsDone() == false)
   {
      CPPUNIT_ASSERT(Iterator.GetItem(&p_CurHeader) == 0);
      CPPUNIT_ASSERT(p_CurHeader != NULL);

      CPPUNIT_ASSERT(i_Index <= 1);
      switch(i_Index)
      {
         case 0:
            CPPUNIT_ASSERT(p_CurHeader->Name == Header1Name);
            CPPUNIT_ASSERT(p_CurHeader->Value == Header2Val);
         break;
         case 1:
            CPPUNIT_ASSERT(p_CurHeader->Name == Header2Name);
            CPPUNIT_ASSERT(p_CurHeader->Value == Header2Val);
         break;
      }
      i_Index++;
      Iterator.Next();
   }

};



