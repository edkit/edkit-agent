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
* @date     2013/07/01
*
*****************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "SliceFreeProbe.h"
#include "ExeContext.h"

extern "C" {

void g_slice_free1(gsize block_size, gpointer mem_block)
{
   mock().actualCall("g_slice_free1")
      .withParameter("block_size", (int)block_size)
      .withParameter("mem_block", mem_block);
}

void xunit_slice_free(gsize block_size, gpointer mem_block)
{
   mock().actualCall("xunit_slice_free")
      .withParameter("block_size", (int)block_size)
      .withParameter("mem_block", mem_block);
}

}

TEST_GROUP(SliceFreeTestGroup)
{
   void teardown()
   {
      mock().clear();
   }
};


TEST(SliceFreeTestGroup, Build)
{
   GLib::SliceFreeProbe  Probe;
   GLib::SliceFreeProbe  *p_Probe;

   p_Probe = new(std::nothrow) GLib::SliceFreeProbe();

   CHECK(p_Probe != NULL);

   delete p_Probe;
}

TEST(SliceFreeTestGroup, FreeOK)
{
   CallStack Callers;
   ExeContext *Context = ExeContext::Get(Callers);
   int AllocSize = 31;

   /* Add an entry in the heap and context lists */
   CHECK(Context != NULL);
   Context->UpdateMemory(AllocSize);
   HeapEntry *Entry = new (std::nothrow) HeapEntry(AllocSize, Context);
   CHECK(Entry != NULL);

   MemHeap *Heap = MemHeap::Instantiate();
   CHECK(Heap != NULL);
   Heap->GetEntryList()->AppendItem(Entry);

   mock().expectOneCall("rtsym_resolve")
      .withParameter("Symbol", "g_slice_free1")
      .andReturnValue((void*)g_slice_free1);

   mock().expectOneCall("g_slice_free1")
      .withParameter("block_size", (int)(AllocSize+sizeof(HeapEntry)))
      .withParameter("mem_block", Entry);

   /* Free the entry */
   GLib::SliceFreeProbe  Probe;
   Probe.InitCheck();

   uint8_t *Data = (uint8_t*)Entry;
   Probe.Free(AllocSize, Data+sizeof(HeapEntry));
   CHECK_EQUAL(0, (int)Context->GetMemory() );

   mock().checkExpectations();
   ExeContext::Reset();
   delete Entry;
}

TEST(SliceFreeTestGroup, FreeNullFunc)
{
   GLib::SliceFreeProbe  Probe;

   Probe.Free(31, NULL);
   mock().checkExpectations();
}

TEST(SliceFreeTestGroup, FreeNull)
{
   int AllocSize = 31;

   mock().expectOneCall("rtsym_resolve")
      .withParameter("Symbol", "g_slice_free1")
      .andReturnValue((void*)g_slice_free1);

   mock().expectOneCall("g_slice_free1")
      .withParameter("block_size", AllocSize)
      .withParameter("mem_block", (void*)NULL);

   /* Free the entry */
   GLib::SliceFreeProbe  Probe;
   Probe.InitCheck();

   Probe.Free(AllocSize, NULL);
   mock().checkExpectations();
}

TEST(SliceFreeTestGroup, FreeHeapLocked)
{
   CallStack Callers;
   ExeContext *Context = ExeContext::Get(Callers);
   int AllocSize = 31;

   /* Add an entry in the heap and context lists */
   CHECK(Context != NULL);
   Context->UpdateMemory(AllocSize);
   HeapEntry *Entry = new (std::nothrow) HeapEntry(AllocSize, Context);
   CHECK(Entry != NULL);

   MemHeap *Heap = MemHeap::Instantiate();
   CHECK(Heap != NULL);
   Heap->Lock();
   Heap->GetEntryList()->AppendItem(Entry);

   mock().expectOneCall("rtsym_resolve")
      .withParameter("Symbol", "g_slice_free1")
      .andReturnValue((void*)g_slice_free1);

   mock().expectOneCall("g_slice_free1")
      .withParameter("block_size", AllocSize)
      .withParameter("mem_block", Entry);

   /* Free the entry */
   GLib::SliceFreeProbe  Probe;
   Probe.InitCheck();

   Probe.Free(AllocSize, Entry);
   CHECK_EQUAL(AllocSize, (int)Context->GetMemory() );

   mock().checkExpectations();
   ExeContext::Reset();
   MemHeap::Instantiate()->Reset();
   delete Entry;
   Heap->Unlock();
}

/* Passthrough without free function as parameter */
TEST(SliceFreeTestGroup, Passthrough1)
{
   int AllocSize = 31;
   int Data;

   mock().expectOneCall("rtsym_resolve")
      .withParameter("Symbol", "g_slice_free1")
      .andReturnValue((void*)g_slice_free1);

   mock().expectOneCall("g_slice_free1")
      .withParameter("block_size", AllocSize)
      .withParameter("mem_block", (void*)&Data);

   /* Free the entry */
   GLib::SliceFreeProbe  Probe;

   Probe.PassThrough(AllocSize, &Data);

   mock().checkExpectations();
}

/* Passthrough without free function as parameter and rtsym_resolve returning
 * NULL */
TEST(SliceFreeTestGroup, Passthrough2)
{
   int AllocSize = 31;
   int Data;

   mock().expectOneCall("rtsym_resolve")
      .withParameter("Symbol", "g_slice_free1")
      .andReturnValue((void*)NULL);

   /* Free the entry */
   GLib::SliceFreeProbe  Probe;

   Probe.PassThrough(AllocSize, &Data);

   mock().checkExpectations();
}


/* Passthrough with free function as parameter */
TEST(SliceFreeTestGroup, Passthrough3)
{
   int AllocSize = 31;
   int Data;

   mock().expectOneCall("rtsym_resolve")
      .withParameter("Symbol", "xunit_slice_free")
      .andReturnValue((void*)xunit_slice_free);

   mock().expectOneCall("xunit_slice_free")
      .withParameter("block_size", AllocSize)
      .withParameter("mem_block", (void*)&Data);

   /* Free the entry */
   GLib::SliceFreeProbe  Probe;

   Probe.PassThrough(AllocSize, &Data, "xunit_slice_free");

   mock().checkExpectations();
}


/* Passthrough with free function as parameter and rtsym_resolve returning
 * NULL */
TEST(SliceFreeTestGroup, Passthrough4)
{
   int AllocSize = 31;
   int Data;

   mock().expectOneCall("rtsym_resolve")
      .withParameter("Symbol", "xunit_slice_free")
      .andReturnValue((void*)NULL);

   /* Free the entry */
   GLib::SliceFreeProbe  Probe;

   Probe.PassThrough(AllocSize, &Data, "xunit_slice_free");

   mock().checkExpectations();
}

