
CFLAGS = -fPIC -g -Wall -Werror -D_GNU_SOURCE
CFLAGS += -DNO_SSL

SRCS = src/Os/rtsym.c
SRCS += src/Http/mongoose/mongoose.c

CXX_SRCS = src/Preload/DlList.cpp
CXX_SRCS += src/DynObject/DynObject.cpp src/DynObject/JsonFlattener.cpp
CXX_SRCS += src/Os/Thread.cpp src/Os/CallStack.cpp
CXX_SRCS += src/Http/Httpd.cpp src/Http/HttpdSingleton.cpp src/Http/HttpdRequest.cpp src/Http/UrlHandler.cpp src/Http/WsMethod.cpp 
CXX_SRCS += src/Http/WsHandler.cpp src/Http/WsHandlerSingleton.cpp
CXX_SRCS += src/Http/WsMethod.cpp src/Http/WsHandler.cpp src/Http/WsInterface.cpp
CXX_SRCS += src/String.cpp

CPPFLAGS = -I./include -I../thirdparty/json-c

NAME = toolbox
BUILD_TYPE = staticlib

include $(BUILDDIR)/srcbuild.mk

