
CFLAGS = -g -Wall -Werror -D_GNU_SOURCE
CPPFLAGS = -I$(SRCDIR)/toolbox/include -I$(SRCDIR)/thirdparty/cppunit/include
CPPFLAGS += -I$(SRCDIR)/thirdparty/cppunit/include
CPPFLAGS += -I$(SRCDIR)/edleak

LDFLAGS = -rdynamic
LDFLAGS += -L$(SRCDIR)/toolbox -ltoolbox
LDFLAGS += -L$(SRCDIR)/edleak -ledleak
LDFLAGS += -L$(SRCDIR)/thirdparty/json-c/.libs -ljson
LDFLAGS += -L$(SRCDIR)/thirdparty/cppunit/src/cppunit/.libs -lcppunit

SRCS =

CXX_SRCS = main.cpp
CXX_SRCS += FakeAlloc.cpp
CXX_SRCS += MemAlign1.cpp
CXX_SRCS += TestMemAlloc.cpp


NAME = edleak_utest
BUILD_TYPE = exe

run:
	LD_LIBRARY_PATH=$(SRCDIR)/edleak $(CURDIR)/$(NAME)

include $(BUILDDIR)/srcbuild.mk

