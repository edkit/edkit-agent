AM_CPPFLAGS = -D_GNU_SOURCE -DNO_SSL -I$(top_srcdir)/include/edkit/toolbox -I$(top_builddir)/include/edkit/toolbox
AM_CPPFLAGS += $(JSONC_CFLAGS)

noinst_LTLIBRARIES = libedkit-toolbox.la

libedkit_toolbox_la_SOURCES = \
    OakString.cpp \
    DynObject/DynObject.cpp \
    DynObject/JsonFlattener.cpp \
    Http/Httpd.cpp \
    Http/HttpdRequest.cpp \
    Http/HttpdSingleton.cpp \
    Http/UrlHandler.cpp \
    Http/WsHandler.cpp \
    Http/WsHandlerSingleton.cpp \
    Http/WsInterface.cpp \
    Http/WsMethod.cpp \
    Http/mongoose/mongoose.c \
    Os/CallStack.cpp \
    Os/rtsym.c \
    Os/Thread.cpp \
    Preload/DlList.cpp

libedkit_toolbox_la_CPPFLAGS = $(AM_CPPFLAGS)

libedkit_toolbox_la_CFLAGS = -fPIC
libedkit_toolbox_la_CXXFLAGS = -fPIC
