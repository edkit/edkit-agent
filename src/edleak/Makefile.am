
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

libedleak_la_CPPFLAGS = -D_GNU_SOURCE -I$(top_srcdir)/include/edkit/toolbox -I$(srcdir)/heap -I$(srcdir)/probe
libedleak_la_CPPFLAGS += -DFW_DEFAULT_FILENAME=\"edleak.json\"
libedleak_la_CPPFLAGS += -DFW_DEFAULT_POLL_PERIOD=30

libedleak_la_CXXFLAGS = -fPIC

libedleak_la_LIBADD = ../toolbox/libedkit-toolbox.a
libedleak_la_LIBADD += $(JSON_LIBS)

