
CFLAGS = -fPIC -g -Wall -Werror -D_GNU_SOURCE
CFLAGS += -DNO_SSL

SRCS = Os/rtsym.c
SRCS += Http/mongoose/mongoose.c

CXX_SRCS = Preload/DlList.cpp
CXX_SRCS += DynObject/DynObject.cpp DynObject/JsonFlattener.cpp
CXX_SRCS += Os/Thread.cpp Os/CallStack.cpp
CXX_SRCS += Http/Httpd.cpp Http/HttpdSingleton.cpp Http/HttpdRequest.cpp Http/UrlHandler.cpp Http/WsMethod.cpp 
CXX_SRCS += Http/WsHandler.cpp Http/WsHandlerSingleton.cpp
CXX_SRCS += Http/WsMethod.cpp Http/WsHandler.cpp Http/WsInterface.cpp
CXX_SRCS += String.cpp

CPPFLAGS = -I./include -I../thirdparty/json-c

NAME = toolbox
BUILD_TYPE = staticlib

include $(BUILDDIR)/srcbuild.mk

