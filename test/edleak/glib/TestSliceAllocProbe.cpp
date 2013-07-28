/*
*****************************************************************************
*                      ___       _   _    _ _
*                     / _ \ __ _| |_| |__(_) |_ ___
*                    | (_) / _` | / / '_ \ |  _(_-<
*                     \___/\__,_|_\_\_.__/_|\__/__/
*                          Copyright (c) 2013
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
* @date     2013/07/11
*
*****************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include <malloc.h>
#include "CallStack.h"
#include "SliceAllocProbe.h"
#include "ExeContext.h"

gpointer g_slice_alloc(gsize block_size)
{
   mock().actualCall("g_slice_alloc")
      .withParameter("block_size", (int)block_size);
   return mock().pointerReturnValue();
}

gpointer xunit_slice_alloc(gsize block_size)
{
   mock().actualCall("xunit_slice_alloc")
      .withParameter("block_size", (int)block_size);
   return mock().pointerReturnValue();
}

gpointer xunit_slice_alloc2(gsize block_size)
{
   mock().actualCall("xunit_slice_alloc2");
   return mock().pointerReturnValue();
}

TEST_GROUP(SliceAllocTestGroup)
{
   void teardown()
   {
      mock().clear();
   }
};


TEST(SliceAllocTestGroup, Build)
{
   GLib::SliceAllocProbe  Probe;
   GLib::SliceAllocProbe  *p_Probe;

   p_Probe = new(std::nothrow) GLib::SliceAllocProbe();
   CHECK(p_Probe != NULL);
   delete p_Probe;
}

/* Passthrough without function as parameter */
TEST(SliceAllocTestGroup, Passthrough1)
{
   int AllocSize = 31;
   char Data[31];

   mock().expectOneCall("rtsym_resolve")
      .withParameter("Symbol", "g_slice_alloc")
      .andReturnValue((void*)g_slice_alloc);

   mock().expectOneCall("g_slice_alloc")
      .withParameter("block_size", AllocSize)
      .andReturnValue((void*)Data);

   /* Free the entry */
   GLib::SliceAllocProbe  Probe;
   void *ProbeData;

   ProbeData = Probe.PassThrough(AllocSize);
   CHECK_EQUAL(Data, ProbeData);
   mock().checkExpectations();
}

/* Passthrough without function as parameter and rtsym_resolve returning
 * NULL */
TEST(SliceAllocTestGroup, Passthrough2)
{
   int AllocSize = 31;

   mock().expectOneCall("rtsym_resolve")
      .withParameter("Symbol", "g_slice_alloc")
      .andReturnValue((void*)NULL);

   /* Free the entry */
   GLib::SliceAllocProbe  Probe;
   void *ProbeData;

   ProbeData = Probe.PassThrough(AllocSize);
   CHECK_EQUAL((void*)NULL, ProbeData);
   mock().checkExpectations();
}


/* Passthrough with function as parameter */
TEST(SliceAllocTestGroup, Passthrough3)
{
   int AllocSize = 31;
   char Data[31];

   mock().expectOneCall("rtsym_resolve")
      .withParameter("Symbol", "xunit_slice_alloc")
      .andReturnValue((void*)xunit_slice_alloc);

   mock().expectOneCall("xunit_slice_alloc")
      .withParameter("block_size", AllocSize)
      .andReturnValue((void*)Data);

   /* Free the entry */
   GLib::SliceAllocProbe  Probe;
   void *ProbeData;

   ProbeData = Probe.PassThrough(AllocSize, "xunit_slice_alloc");
   CHECK_EQUAL(Data, ProbeData);
   mock().checkExpectations();
}


/* Passthrough with free function as parameter and rtsym_resolve returning
 * NULL */
TEST(SliceAllocTestGroup, Passthrough4)
{
   int AllocSize = 31;

   mock().expectOneCall("rtsym_resolve")
      .withParameter("Symbol", "xunit_slice_alloc")
      .andReturnValue((void*)NULL);

   /* Free the entry */
   GLib::SliceAllocProbe  Probe;
   void *ProbeData;

   ProbeData = Probe.PassThrough(AllocSize, "xunit_slice_alloc");
   CHECK_EQUAL((void*)NULL, ProbeData);
   mock().checkExpectations();
}


TEST(SliceAllocTestGroup, AllocOk)
{
   int AllocSize = 31;
   char *Data= (char*)malloc(512);

   CHECK(Data != NULL);

   mock().expectOneCall("rtsym_resolve")
      .withParameter("Symbol", "g_slice_alloc")
      .andReturnValue((void*)xunit_slice_alloc2);

   mock().expectOneCall("xunit_slice_alloc2")
      .andReturnValue((void*)Data);

   GLib::SliceAllocProbe  Probe;
   Probe.InitCheck();

   CallStack Caller;
   UnwindCaller(Caller);
   char *ProbeAddress = (char*)Probe.Alloc(AllocSize, Caller);
   CHECK(ProbeAddress >= Data);
   CHECK((uint64_t)(intptr_t)ProbeAddress % ALLOC_ALIGNMENT == 0);
   CHECK(ProbeAddress < Data+sizeof(HeapEntry)+ALLOC_ALIGNMENT);

   mock().checkExpectations();
   ExeContext::Reset();
   MemHeap::Instantiate()->Reset();
   free(Data);
}



