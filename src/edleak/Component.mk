
CFLAGS = -fPIC -g -Wall -Werror -D_GNU_SOURCE
CPPFLAGS = -DFW_DEFAULT_FILENAME=\"edleak.json\"
CPPFLAGS += -DFW_DEFAULT_POLL_PERIOD=30

LDFLAGS = -L$(SRCDIR)/toolbox -ltoolbox
LDFLAGS += -L$(SRCDIR)/thirdparty/json-c/.libs -ljson
LDFLAGS += -l$(LDLIB_DL) -l$(LDLIB_PTHREAD)
SRCS = 
CXX_SRCS = preload_alloc.cpp  ExeContext.cpp \
           Probe.cpp MemProbe.cpp MemAllocProbe.cpp MemAlignProbe.cpp MemReallocProbe.cpp MemCallocProbe.cpp MemFreeProbe.cpp \
           MemHeap.cpp
CXX_SRCS += ContextCallStackList.cpp
CXX_SRCS += controller/ContextUtils.cpp
CXX_SRCS += controller/Controllers.cpp controller/FileWriter.cpp
CXX_SRCS += controller/WebService.cpp controller/WsMethodSlice.cpp controller/WsMethodStackWatchAdd.cpp

CPPFLAGS += -I$(SRCDIR)/toolbox/include -I$(CURDIR)
NAME = edleak
BUILD_TYPE = dynlib

include $(BUILDDIR)/srcbuild.mk

