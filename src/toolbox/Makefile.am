noinst_LIBRARIES = libedkit-toolbox.a

libedkit_toolbox_a_SOURCES = \
    String.cpp \
    DynObject/DynObject.cpp \
    DynObject/JsonFlattener.cpp \
    Http/Httpd.cpp \
    Http/HttpdRequest.cpp \
    Http/HttpdSingleton.cpp \
    Http/UrlHandler.cpp \
    Http/WsHandlerSingleton.cpp \
    Http/WsInterface.cpp \
    Http/WsMethod.cpp \
    Http/mongoose/mongoose.c \
    Os/CallStack.cpp \
    Os/rtsym.c \
    Os/Thread.cpp \
    Preload/DlList.cpp

libedkit_toolbox_a_CPPFLAGS = -D_GNU_SOURCE -I$(top_srcdir)/include/edkit/toolbox $(JSON_CFLAGS)

