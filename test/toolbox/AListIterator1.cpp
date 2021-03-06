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
#include "ListIterator.h"

TEST_GROUP(AListIteratorTestGroup)
{
};


TEST(AListIteratorTestGroup, Basic)
{
   uint32_t i,j;

   AList<uint32_t> List;
   ListIterator<AList<uint32_t>, uint32_t> Iterator;

   for(i=0; i<1234; i++)
   {
      CHECK_EQUAL(0, List.AddItem(i));
      CHECK_EQUAL(i+1, List.CountItems() );
   }

   CHECK_EQUAL(0, Iterator.SetTo(&List));
   CHECK_EQUAL(0, Iterator.First());
   i=0;
   while(Iterator.IsDone() == false)
   {
      CHECK_EQUAL(0, Iterator.GetItem(&j));
      CHECK_EQUAL(i, j);
      CHECK_EQUAL(0, Iterator.Next());
      i++;
   }
}
