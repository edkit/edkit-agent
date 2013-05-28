
CFLAGS = -fPIC -g -Wall -Werror -D_GNU_SOURCE
CPPFLAGS = -DFW_DEFAULT_FILENAME=\"edleak.json\"
CPPFLAGS += -DFW_DEFAULT_POLL_PERIOD=30

LDFLAGS = -L$(SRCDIR)/toolbox -ltoolbox
LDFLAGS += -L$(SRCDIR)/thirdparty/json-c/.libs -ljson
SRCS = 
CXX_SRCS = src/ContextUtils.cpp
CXX_SRCS += src/Controllers.cpp src/FileWriter.cpp
CXX_SRCS += src/WebService.cpp src/WsMethodSlice.cpp src/WsMethodStackWatchAdd.cpp

CPPFLAGS += -I$(SRCDIR)/toolbox/include
CPPFLAGS += -I$(CURDIR)/../heap/include
NAME = edleak_controller
BUILD_TYPE = staticlib

include $(BUILDDIR)/srcbuild.mk

