
CFLAGS = -fPIC -g -Wall -Werror -D_GNU_SOURCE
SRCS = 
CXX_SRCS = src/Posix.cpp
ifeq ($(SUPPORT_GLIB),yes)
CXX_SRCS += src/Glib.cpp
endif

CPPFLAGS += -I$(SRCDIR)/toolbox/include
CPPFLAGS += -I$(CURDIR)/../heap/include
CPPFLAGS += -I$(CURDIR)/../probe/include

ifeq ($(SUPPORT_GLIB),yes)
CPPFLAGS +=$(CPPFLAGS_GLIB)
LDFLAGS += $(LDFLAGS_GLIB)
endif

NAME = edleak_preload
BUILD_TYPE = staticlib

include $(BUILDDIR)/srcbuild.mk

