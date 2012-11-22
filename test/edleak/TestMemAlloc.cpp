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
* @date     2012/11/21
*
*****************************************************************************/
#include <malloc.h>
#include "MemAllocProbe.h"
#include "TestMemAlloc.h"
#include "FakeAlloc.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TestMemAlloc );

void TestMemAlloc::setUp()
{
}


void TestMemAlloc::tearDown()
{
}


void TestMemAlloc::TestBuild()
{
   MemAllocProbe  Probe;
   MemAllocProbe  *p_Probe;

   Probe.InitCheck();
   p_Probe = new(std::nothrow) MemAllocProbe();

   CPPUNIT_ASSERT(p_Probe != NULL);
   p_Probe->InitCheck();

   delete p_Probe;
}

void TestMemAlloc::TestAlign()
{
   MemAllocProbe  Probe;
   Probe.InitCheck(FakeAlloc_Malloc);

   char *SysAddress = (char*)malloc(512);
   CPPUNIT_ASSERT(SysAddress != NULL);

   FakeAlloc_SetAllocAddress(SysAddress);
   char *ProbeAddress = (char*)Probe.Alloc(25, NULL);
   CPPUNIT_ASSERT(ProbeAddress >= SysAddress);
   CPPUNIT_ASSERT((uint64_t)(intptr_t)ProbeAddress % ALLOC_ALIGNMENT == 0);
   CPPUNIT_ASSERT(ProbeAddress < SysAddress+sizeof(HeapEntry)+ALLOC_ALIGNMENT);
}



