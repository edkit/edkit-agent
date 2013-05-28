
CFLAGS = -g -Wall -Werror -D_GNU_SOURCE
CPPFLAGS = -I$(SRCDIR)/toolbox/include -I$(SRCDIR)/thirdparty/cppunit/include
CPPFLAGS += -I$(SRCDIR)/thirdparty/cpputest/include
CPPFLAGS += -I$(SRCDIR)/edleak/probe/include
CPPFLAGS += -I$(SRCDIR)/edleak/heap/include

LDFLAGS =
LDFLAGS += -L$(SRCDIR)/thirdparty/cpputest/lib
LDFLAGS += -lCppUTest -lCppUTestExt
LDFLAGS += -L$(SRCDIR)/edleak/probe -ledleak_probe
LDFLAGS += -L$(SRCDIR)/edleak/heap -ledleak_heap
LDFLAGS += -L$(SRCDIR)/toolbox -ltoolbox
LDFLAGS += -L$(SRCDIR)/thirdparty/json-c/.libs -ljson
LDFLAGS += -l$(LDLIB_DL)

SRCS =

CXX_SRCS = main.cpp
CXX_SRCS += FakeAlloc.cpp
CXX_SRCS += MemAlign1.cpp
CXX_SRCS += TestMemAlloc.cpp
CXX_SRCS += TestExeContext.cpp


NAME = edleak_utest
BUILD_TYPE = exe

run:
	LD_LIBRARY_PATH=$(SRCDIR)/edleak $(CURDIR)/$(NAME)

include $(BUILDDIR)/srcbuild.mk

