
CFLAGS = -fPIC -g -Wall -Werror -D_GNU_SOURCE

LDFLAGS = -L$(SRCDIR)/toolbox -ltoolbox
SRCS = 
CXX_SRCS = src/ExeContext.cpp src/MemHeap.cpp src/ContextCallStackList.cpp

CPPFLAGS = -I$(SRCDIR)/toolbox/include
NAME = edleak_heap
BUILD_TYPE = staticlib

include $(BUILDDIR)/srcbuild.mk

