

LDFLAGS = -L./controller -Wl,--whole-archive -ledleak_controller -Wl,--no-whole-archive
LDFLAGS += -L./preload -Wl,--whole-archive -ledleak_preload -Wl,--no-whole-archive
LDFLAGS += -L./probe -ledleak_probe
LDFLAGS += -L./heap -ledleak_heap
LDFLAGS += -L$(SRCDIR)/toolbox -ltoolbox
LDFLAGS += -L$(SRCDIR)/thirdparty/json-c/.libs -ljson
LDFLAGS += -l$(LDLIB_DL) -l$(LDLIB_PTHREAD)
SRCS = 
CXX_SRCS = 

NAME = edleak
BUILD_TYPE = dynlib
SUB_COMPONENTS:=heap probe controller preload

include $(BUILDDIR)/srcbuild.mk

