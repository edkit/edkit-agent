SRCS = Os/rtsym.c
#SRCS += Http/mongoose/mongoose.c

CXX_SRCS = Preload/DlList.cpp
CXX_SRCS += DynObject/DynObject.cpp
CXX_SRCS += Os/Thread.cpp
CXX_SRCS += Http/Httpd.cpp Http/UrlHandler.cpp Http/WsMethod.cpp Http/WsHandler.cpp
CXX_SRCS += Http/WsMethod.cpp Http/WsHandler.cpp Http/WsInterface.cpp
CXX_SRCS += String.cpp

CPPFLAGS = -I./include

NAME = toolbox
BUILD_TYPE = staticlib

include ../../build/arch.make
include flags.make
include ../../build/makefile.gen

