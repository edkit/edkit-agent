include ../../build/arch.make
include flags.make

SRCS = 
CXX_SRCS = preload_alloc.cpp  ExeContext.cpp \
           Probe.cpp MemProbe.cpp MemAllocProbe.cpp MemAlignProbe.cpp MemReallocProbe.cpp MemCallocProbe.cpp MemFreeProbe.cpp \
           MemHeap.cpp
CXX_SRCS += controller/ContextUtils.cpp
CXX_SRCS += controller/Controllers.cpp controller/FileWriter.cpp
CXX_SRCS += controller/WebService.cpp controller/WsMethodSlice.cpp

CPPFLAGS += -I../toolbox/include -I$(CURDIR)
NAME = edleak
BUILD_TYPE = dynlib

include ../../build/makefile.gen

