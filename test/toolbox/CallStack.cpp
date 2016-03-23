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
#include "CppUTest/TestHarness.h"
#include <dlfcn.h>
#include <string.h>
#include "CallStack.h"

TEST_GROUP(CallStackTestGroup)
{
};


TEST(CallStackTestGroup, Build)
{
   CallStack   TestCallStack1, *TestCallStack2;

   TestCallStack2 = new CallStack();
   CHECK(TestCallStack2  != NULL);
   delete TestCallStack2;
};

extern "C"
{
void __attribute__((noinline)) TestSuiteCaller(CallStack &Unwinder)
{
   UnwindCaller(Unwinder);
}

void __attribute__((noinline)) TestSuiteCaller1(CallStack &Unwinder)
{
   TestSuiteCaller(Unwinder);
}

void __attribute__((noinline)) TestSuiteUnwind(CallStack &Unwinder)
{
   Unwinder.Unwind();
}
void __attribute__((noinline)) TestSuiteUnwind1(CallStack &Unwinder)
{
   TestSuiteUnwind(Unwinder);
}
void __attribute__((noinline)) TestSuiteUnwind2(CallStack &Unwinder)
{
   TestSuiteUnwind1(Unwinder);
}
void __attribute__((noinline)) TestSuiteUnwind3(CallStack &Unwinder)
{
   TestSuiteUnwind2(Unwinder);
}
void __attribute__((noinline)) TestSuiteUnwind4(CallStack &Unwinder)
{
   TestSuiteUnwind3(Unwinder);
}
}

void __attribute__((noinline)) TestSuiteUnwind5(CallStack &Unwinder)
{
   TestSuiteUnwind4(Unwinder);
}

TEST(CallStackTestGroup, GetDepth)
{
   CallStack   TestCallStack;

   CHECK_EQUAL(CALLSTACK_MAX_DEPTH, TestCallStack.GetDepth());
};

TEST(CallStackTestGroup, UnwindOneCaller)
{
   CallStack   TestCallStack;

   TestSuiteCaller1(TestCallStack);

   const void **Stack = TestCallStack.Get();
   Dl_info Info;
   CHECK(dladdr(Stack[0], &Info) != 0);
   CHECK_EQUAL(0, strcmp(Info.dli_sname, "TestSuiteCaller1"));
};

TEST(CallStackTestGroup, Unwind)
{
   CallStack   TestCallStack;

   TestSuiteUnwind5(TestCallStack);

   const void **Stack = TestCallStack.Get();
   Dl_info Info;
   CHECK(dladdr(Stack[0], &Info) != 0);
   CHECK_EQUAL(0, strcmp(Info.dli_sname, "TestSuiteUnwind1"));
   CHECK(dladdr(Stack[1], &Info) != 0);
   CHECK_EQUAL(0, strcmp(Info.dli_sname, "TestSuiteUnwind2"));
   CHECK(dladdr(Stack[2], &Info) != 0);
   CHECK_EQUAL(0, strcmp(Info.dli_sname, "TestSuiteUnwind3"));
   CHECK(dladdr(Stack[3], &Info) != 0);
   CHECK_EQUAL(0, strcmp(Info.dli_sname, "TestSuiteUnwind4"));
   CHECK(dladdr(Stack[4], &Info) != 0);
   CHECK(strstr(Info.dli_sname, "TestSuiteUnwind5") != NULL);
};

TEST(CallStackTestGroup, SetTo)
{
   CallStack   TestCallStack;
   CallStack   TestCallStack2;

   TestSuiteUnwind5(TestCallStack);
   TestCallStack2.SetTo(TestCallStack);

   CHECK_EQUAL(0, memcmp(TestCallStack.Get(),
                TestCallStack2.Get(),
                CALLSTACK_MAX_DEPTH*sizeof(void*)));
};

TEST(CallStackTestGroup, OperatorEqual)
{
   CallStack   TestCallStack;
   CallStack   TestCallStack2;
   CallStack   TestCallStack3;

   TestSuiteUnwind5(TestCallStack);
   TestCallStack2.SetTo(TestCallStack);
   TestSuiteUnwind4(TestCallStack3);

   CHECK(TestCallStack == TestCallStack2);
   CHECK(!(TestCallStack == TestCallStack3));
};

TEST(CallStackTestGroup, OperatorDifferent)
{
   CallStack   TestCallStack;
   CallStack   TestCallStack2;
   CallStack   TestCallStack3;

   TestSuiteUnwind5(TestCallStack);
   TestSuiteUnwind4(TestCallStack2);
   TestCallStack3.SetTo(TestCallStack);

   CHECK(TestCallStack != TestCallStack2);
   CHECK(!(TestCallStack != TestCallStack3));
};

TEST(CallStackTestGroup, GetNameBigLevel)
{
   CallStack   TestCallStack;

   TestSuiteUnwind5(TestCallStack);

   POINTERS_EQUAL(NULL, TestCallStack.GetName(300000));
};


TEST(CallStackTestGroup, GetName)
{
   CallStack   TestCallStack;

   TestSuiteUnwind5(TestCallStack);

   /* check for names, format is "pointer:name" so we compare the name for
    * symbols that should be resolved */
   CHECK_EQUAL(0, strcmp(strchr(TestCallStack.GetName(0), ':')+1, "TestSuiteUnwind1"));
   CHECK_EQUAL(0, strcmp(strchr(TestCallStack.GetName(1), ':')+1, "TestSuiteUnwind2"));
   CHECK_EQUAL(0, strcmp(strchr(TestCallStack.GetName(2), ':')+1, "TestSuiteUnwind3"));
   CHECK_EQUAL(0, strcmp(strchr(TestCallStack.GetName(3), ':')+1, "TestSuiteUnwind4"));
   CHECK_EQUAL(0, strcmp(strchr(TestCallStack.GetName(4), ':')+1, "TestSuiteUnwind5(CallStack&)"));
};

TEST(CallStackTestGroup, GetNameCaller)
{
   CallStack   TestCallStack;

   TestSuiteCaller1(TestCallStack);

   /* check for names, format is "pointer:name" so we compare the name for
    * symbols that should be resolved */
   CHECK_EQUAL(0, strcmp(strchr(TestCallStack.GetName(0), ':')+1, "TestSuiteCaller1"));
   uint32_t Level;
   for(Level=1;Level<TestCallStack.GetDepth(); Level++)
   {
      POINTERS_EQUAL(NULL, TestCallStack.GetName(Level));
   }
};

TEST(CallStackTestGroup, GetSoName)
{
   CallStack   TestCallStack;

   TestSuiteUnwind5(TestCallStack);
   STRCMP_EQUAL("utest_edkit_toolbox", strstr(TestCallStack.GetSoName(0), "utest_edkit_toolbox"));
};
