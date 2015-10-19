
lib_LTLIBRARIES = libedleak.la

glib_sources = \
    preload/Glib.cpp \
    probe/glib/SliceAllocProbe.cpp \
    probe/glib/SliceFreeProbe.cpp

libedleak_la_SOURCES = \
    controller/ContextUtils.cpp \
    controller/Controllers.cpp \
    controller/FileWriter.cpp \
    controller/WebService.cpp \
    controller/WsMethodSlice.cpp \
    controller/WsMethodStackWatchAdd.cpp \
    heap/ContextCallStackList.cpp \
    heap/ExeContext.cpp \
    heap/MemHeap.cpp \
    preload/Posix.cpp \
    probe/MemAlignProbe.cpp \
    probe/MemAllocProbe.cpp \
    probe/MemCallocProbe.cpp \
    probe/MemFreeProbe.cpp \
    probe/MemProbe.cpp \
    probe/MemReallocProbe.cpp \
    probe/Probe.cpp

#libheliostv_la_CPPFLAGS = -I$(top_srcdir)/include $(BOOST_CPPFLAGS)
libedleak_la_LIBADD = $(JSON_LIBS) $(PTHREAD_LIBS)
#libheliostv_la_LDFLAGS = 
