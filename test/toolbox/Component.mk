
CFLAGS = -g -Wall -Werror -D_GNU_SOURCE
CPPFLAGS = -I$(SRCDIR)/toolbox/include -I$(SRCDIR)/thirdparty/cppunit/include
CPPFLAGS += -I$(SRCDIR)/thirdparty/cpputest/include

LDFLAGS =  -l$(LDLIB_DL)
LDFLAGS += -Wl,--export-dynamic
LDFLAGS += -L$(SRCDIR)/thirdparty/cpputest/lib
LDFLAGS += -lCppUTest -lCppUTestExt
LDFLAGS += -L$(SRCDIR)/toolbox -ltoolbox
LDFLAGS += -L$(SRCDIR)/thirdparty/json-c/.libs -ljson

SRCS =

CXX_SRCS = main.cpp
CXX_SRCS += CallStack.cpp
CXX_SRCS += AList1.cpp
CXX_SRCS += AListIterator1.cpp
CXX_SRCS += HttpdRequest1.cpp
CXX_SRCS += String1.cpp
CXX_SRCS += DynObject1.cpp
CXX_SRCS += JsonFlattener1.cpp


NAME = toolbox_utest
BUILD_TYPE = exe

run:
	$(CURDIR)/$(NAME)

include $(BUILDDIR)/srcbuild.mk

