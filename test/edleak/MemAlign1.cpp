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
#include <malloc.h>
#include "CallStack.h"
#include "MemAlignProbe.h"
#include "FakeAlloc.h"
#include "ExeContext.h"

TEST_GROUP(MemAlignTestGroup)
{
};


TEST(MemAlignTestGroup, Build)
{
   MemAlignProbe  Probe;
   MemAlignProbe  *p_Probe;

   p_Probe = new(std::nothrow) MemAlignProbe();

   CHECK(p_Probe != NULL);

   delete p_Probe;
}


TEST(MemAlignTestGroup, Basic)
{
   MemAlignProbe  Probe;
   Probe.InitCheck(FakeAlloc_Memalign);

   char *SysAddress = (char*)memalign(4, 512);
   CHECK(SysAddress != NULL);

   FakeAlloc_SetAllocAddress(SysAddress);

   CallStack Caller;
   UnwindCaller(Caller);
   char *ProbeAddress = (char*)Probe.MemAlign(4, 259, Caller);
   CHECK(ProbeAddress >= SysAddress);
   CHECK((uint64_t)(intptr_t)ProbeAddress % 4 == 0);
   CHECK(ProbeAddress < SysAddress+sizeof(HeapEntry)+4);

   ExeContext::Reset();
}


/*
void MemAlign1::TestPassthrough()
{
   MemAlignProbe  Probe;
   Probe.InitCheck(FakeAlloc_Memalign);

   char *SysAddress = (char*)memalign(4, 512);
   CPPUNIT_ASSERT(SysAddress != NULL);

   FakeAlloc_SetAllocAddress(SysAddress);
   char *ProbeAddress = (char*)Probe.PassThrough(4, 259, "FakeAlloc_Memalign");
   CPPUNIT_ASSERT(ProbeAddress == SysAddress);
}
*/


TEST(MemAlignTestGroup, BigAlign)
{
   MemAlignProbe  Probe;
   Probe.InitCheck(FakeAlloc_Memalign);

   char *SysAddress = (char*)memalign(256, 1024);
   CHECK(SysAddress != NULL);
   FakeAlloc_SetAllocAddress(SysAddress);

   CallStack Caller;
   UnwindCaller(Caller);
   char *ProbeAddress = (char*)Probe.MemAlign(256, 259, Caller);
   CHECK(ProbeAddress >= SysAddress);
   CHECK((uint64_t)(intptr_t)ProbeAddress % 256 == 0);
   CHECK(ProbeAddress < SysAddress+sizeof(HeapEntry)+256);

   ExeContext::Reset();
}
