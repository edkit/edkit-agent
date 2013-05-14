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
#include "AList.h"


TEST_GROUP(AListTestGroup)
{
};


TEST(AListTestGroup, Basic)
{
   AList<int32_t> List;

   CHECK_EQUAL(0, List.CountItems());
   CHECK_EQUAL(0, List.AddItem(1));
   CHECK_EQUAL(0, List.AddItem(2));
   CHECK_EQUAL(0, List.AddItem(3));
   CHECK_EQUAL(0, List.AddItem(4));
   CHECK_EQUAL(4, List.CountItems());

   int32_t i;
   CHECK_EQUAL(-1, List.GetItemAt(4, &i));
   CHECK_EQUAL(0, List.GetItemAt(2, &i));
   CHECK_EQUAL(3, i);

   CHECK_EQUAL(0, List.DelItem(2));
   CHECK_EQUAL(3, List.CountItems());
   CHECK_EQUAL(-1, List.GetItemAt(3, &i));
   CHECK_EQUAL(0, List.GetItemAt(2, &i));
   CHECK_EQUAL(4, i);
   CHECK_EQUAL(0, List.GetItemAt(1, &i));
   CHECK_EQUAL(3, i);
   CHECK_EQUAL(0, List.GetItemAt(0, &i));
   CHECK_EQUAL(1, i);

   CHECK_EQUAL(0, List.DelItem(1));
   CHECK_EQUAL(2, List.CountItems());
   CHECK_EQUAL(-1, List.DelItem(2));
};

TEST(AListTestGroup, IntResize)
{
   uint32_t i,j;

   AList<uint32_t> List;
   for(i=0; i<1234; i++)
   {
      CHECK_EQUAL(0, List.AddItem(i));
      CHECK_EQUAL(i+1, List.CountItems());
   }
   for(i=0; i<1234; i++)
   {
      CHECK_EQUAL(0, List.GetItemAt(i, &j));
      CHECK_EQUAL(j, i);
   }

}


TEST(AListTestGroup, HasItem)
{
   AList<int32_t> List;

   CHECK_EQUAL(0, List.AddItem(1));
   CHECK_EQUAL(0, List.AddItem(2));
   CHECK_EQUAL(true, List.HasItem(1));
   CHECK_EQUAL(true, List.HasItem(2));
   CHECK_EQUAL(false, List.HasItem(3));

   CHECK_EQUAL(0, List.DelItem(1));
   CHECK_EQUAL(false, List.HasItem(1));
};

