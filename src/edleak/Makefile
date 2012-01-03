include ../../build/arch.make
include flags.make

SRCS = 
CXX_SRCS = preload_alloc.cpp Controllers.cpp ExeContext.cpp \
           Probe.cpp MemProbe.cpp MemAllocProbe.cpp MemAlignProbe.cpp MemReallocProbe.cpp MemCallocProbe.cpp MemFreeProbe.cpp \
           MemHeap.cpp \
           FileWriter.cpp

CPPFLAGS += -I../toolbox/include
NAME = edleak
BUILD_TYPE = dynlib

include ../../build/makefile.gen

