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
* @date     2013/02/02
*
*****************************************************************************/
#include <cppunit/extensions/HelperMacros.h>
#include <dlfcn.h>
#include <string.h>
#include "CallStack.h"


class CallStackTestSuite: public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( CallStackTestSuite );
  CPPUNIT_TEST( Build );
  CPPUNIT_TEST( GetDepth );
  CPPUNIT_TEST( UnwindCaller );
  CPPUNIT_TEST( Unwind );
  CPPUNIT_TEST( SetTo );
  CPPUNIT_TEST( OperatorEqual );
  CPPUNIT_TEST( OperatorDifferent );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void Build();
  void GetDepth();
  void UnwindCaller();
  void Unwind();
  void SetTo();
  void OperatorEqual();
  void OperatorDifferent();
};


CPPUNIT_TEST_SUITE_REGISTRATION( CallStackTestSuite );

void CallStackTestSuite::setUp()
{
}


void CallStackTestSuite::tearDown()
{
}

void CallStackTestSuite::Build()
{
   CallStack   TestCallStack1, *TestCallStack2;

   TestCallStack2 = new(std::nothrow) CallStack();
   CPPUNIT_ASSERT(TestCallStack2  != NULL);
   delete TestCallStack2;
};

extern "C"
{
void TestSuiteCaller(CallStack &Unwinder)
{
   Unwinder.UnwindCaller();
}

void TestSuiteCaller1(CallStack &Unwinder)
{
   TestSuiteCaller(Unwinder);
}

void TestSuiteUnwind(CallStack &Unwinder)
{
   Unwinder.Unwind();
}
void TestSuiteUnwind1(CallStack &Unwinder)
{
   TestSuiteUnwind(Unwinder);
}
void TestSuiteUnwind2(CallStack &Unwinder)
{
   TestSuiteUnwind1(Unwinder);
}
void TestSuiteUnwind3(CallStack &Unwinder)
{
   TestSuiteUnwind2(Unwinder);
}
void TestSuiteUnwind4(CallStack &Unwinder)
{
   TestSuiteUnwind3(Unwinder);
}
void TestSuiteUnwind5(CallStack &Unwinder)
{
   TestSuiteUnwind4(Unwinder);
}

}

void CallStackTestSuite::GetDepth()
{
   CallStack   TestCallStack;

   CPPUNIT_ASSERT(TestCallStack.GetDepth() == CALLSTACK_MAX_DEPTH);
};

void CallStackTestSuite::UnwindCaller()
{
   CallStack   TestCallStack;

   TestSuiteCaller1(TestCallStack);

   const void **Stack = TestCallStack.Get();
   Dl_info Info;
   CPPUNIT_ASSERT(dladdr(Stack[0], &Info) != 0);
   CPPUNIT_ASSERT(strcmp(Info.dli_sname, "TestSuiteCaller") == 0);
};

void CallStackTestSuite::Unwind()
{
   CallStack   TestCallStack;

   TestSuiteUnwind5(TestCallStack);

   const void **Stack = TestCallStack.Get();
   Dl_info Info;
   CPPUNIT_ASSERT(dladdr(Stack[0], &Info) != 0);
   CPPUNIT_ASSERT(strcmp(Info.dli_sname, "TestSuiteUnwind1") == 0);
   CPPUNIT_ASSERT(dladdr(Stack[1], &Info) != 0);
   CPPUNIT_ASSERT(strcmp(Info.dli_sname, "TestSuiteUnwind2") == 0);
   CPPUNIT_ASSERT(dladdr(Stack[2], &Info) != 0);
   CPPUNIT_ASSERT(strcmp(Info.dli_sname, "TestSuiteUnwind3") == 0);
   CPPUNIT_ASSERT(dladdr(Stack[3], &Info) != 0);
   CPPUNIT_ASSERT(strcmp(Info.dli_sname, "TestSuiteUnwind4") == 0);
   CPPUNIT_ASSERT(dladdr(Stack[4], &Info) != 0);
   CPPUNIT_ASSERT(strcmp(Info.dli_sname, "TestSuiteUnwind5") == 0);
};

void CallStackTestSuite::SetTo()
{
   CallStack   TestCallStack;
   CallStack   TestCallStack2;

   TestSuiteUnwind5(TestCallStack);
   TestCallStack2.SetTo(TestCallStack);

   CPPUNIT_ASSERT(memcmp(TestCallStack.Get(),
                        TestCallStack2.Get(),
                        CALLSTACK_MAX_DEPTH*sizeof(void*)) == 0);
};

void CallStackTestSuite::OperatorEqual()
{
   CallStack   TestCallStack;
   CallStack   TestCallStack2;
   CallStack   TestCallStack3;

   TestSuiteUnwind5(TestCallStack);
   TestCallStack2.SetTo(TestCallStack);
   TestSuiteUnwind4(TestCallStack3);

   CPPUNIT_ASSERT(TestCallStack == TestCallStack2);
   CPPUNIT_ASSERT(!(TestCallStack == TestCallStack3));
};

void CallStackTestSuite::OperatorDifferent()
{
   CallStack   TestCallStack;
   CallStack   TestCallStack2;
   CallStack   TestCallStack3;

   TestSuiteUnwind5(TestCallStack);
   TestSuiteUnwind4(TestCallStack2);
   TestCallStack3.SetTo(TestCallStack);

   CPPUNIT_ASSERT(TestCallStack != TestCallStack2);
   CPPUNIT_ASSERT(!(TestCallStack != TestCallStack3));
};

