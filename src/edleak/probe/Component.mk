
CFLAGS = -fPIC -g -Wall -Werror -D_GNU_SOURCE $(CPPFLAGS_GLIB)

LDFLAGS = -L$(SRCDIR)/toolbox -ltoolbox $(LDFLAGS_GLIB)
SRCS = 
CXX_SRCS = src/Probe.cpp src/MemProbe.cpp \
			  src/MemAllocProbe.cpp src/MemAlignProbe.cpp src/MemReallocProbe.cpp \
			  src/MemCallocProbe.cpp src/MemFreeProbe.cpp 

ifeq ($(SUPPORT_GLIB),yes)
CXX_SRCS+= src/glib/SliceFreeProbe.cpp
endif

CPPFLAGS = -I$(SRCDIR)/toolbox/include
CPPFLAGS += -I$(CURDIR)/../heap/include
NAME = edleak_probe
BUILD_TYPE = staticlib

include $(BUILDDIR)/srcbuild.mk

