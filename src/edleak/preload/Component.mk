
CFLAGS = -fPIC -g -Wall -Werror -D_GNU_SOURCE
SRCS = 
CXX_SRCS = src/Posix.cpp

CPPFLAGS += -I$(SRCDIR)/toolbox/include
CPPFLAGS += -I$(CURDIR)/../heap/include
CPPFLAGS += -I$(CURDIR)/../probe/include
NAME = edleak_preload
BUILD_TYPE = staticlib

include $(BUILDDIR)/srcbuild.mk

