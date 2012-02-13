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
#include "AList.h"
#include "AList1.h"

CPPUNIT_TEST_SUITE_REGISTRATION( AList1 );

void AList1::setUp()
{
}


void AList1::tearDown()
{
}

void AList1::TestIntBasic()
{
   AList<int32_t> List;

   CPPUNIT_ASSERT(List.CountItems() == 0);
   CPPUNIT_ASSERT(List.AddItem(1) == 0);
   CPPUNIT_ASSERT(List.AddItem(2) == 0);
   CPPUNIT_ASSERT(List.AddItem(3) == 0);
   CPPUNIT_ASSERT(List.AddItem(4) == 0);
   CPPUNIT_ASSERT(List.CountItems() == 4);

   int32_t i;
   CPPUNIT_ASSERT(List.GetItemAt(4, &i) == -1);
   CPPUNIT_ASSERT(List.GetItemAt(2, &i) == 0);
   CPPUNIT_ASSERT(i == 3);

   CPPUNIT_ASSERT(List.DelItem(2) == 0);
   CPPUNIT_ASSERT(List.CountItems() == 3);
   CPPUNIT_ASSERT(List.GetItemAt(3, &i) == -1);
   CPPUNIT_ASSERT(List.GetItemAt(2, &i) == 0);
   CPPUNIT_ASSERT(i == 4);
   CPPUNIT_ASSERT(List.GetItemAt(1, &i) == 0);
   CPPUNIT_ASSERT(i == 3);
   CPPUNIT_ASSERT(List.GetItemAt(0, &i) == 0);
   CPPUNIT_ASSERT(i == 1);

   CPPUNIT_ASSERT(List.DelItem(1) == 0);
   CPPUNIT_ASSERT(List.CountItems() == 2);
   CPPUNIT_ASSERT(List.DelItem(2) == -1);
};

void AList1::TestIntResize()
{
   uint32_t i,j;

   AList<uint32_t> List;
   for(i=0; i<1234; i++)
   {
      CPPUNIT_ASSERT(List.AddItem(i) == 0);
      CPPUNIT_ASSERT(List.CountItems() == i+1);
   }
   for(i=0; i<1234; i++)
   {
      CPPUNIT_ASSERT(List.GetItemAt(i, &j) == 0);
      CPPUNIT_ASSERT(i == j);
   }

}
