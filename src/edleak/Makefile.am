AM_CPPFLAGS = -D_GNU_SOURCE -I$(top_srcdir)/include/edkit/toolbox -I$(srcdir)/heap -I$(srcdir)/probe
AM_CXXFLAGS = -fPIC

noinst_LIBRARIES = \
    libedleak-controller.a \
    libedleak-heap.a \
    libedleak-preload.a \
    libedleak-probe.a

lib_LTLIBRARIES = libedleak.la

glib_sources = \
    preload/Glib.cpp \
    probe/glib/SliceAllocProbe.cpp \
    probe/glib/SliceFreeProbe.cpp

libedleak_controller_a_SOURCES = \
    controller/ContextUtils.cpp \
    controller/Controllers.cpp \
    controller/FileWriter.cpp \
    controller/WebService.cpp \
    controller/WsMethodSlice.cpp \
    controller/WsMethodStackWatchAdd.cpp

libedleak_heap_a_SOURCES = \
    heap/ContextCallStackList.cpp \
    heap/ExeContext.cpp \
    heap/MemHeap.cpp

libedleak_preload_a_SOURCES = \
    preload/Posix.cpp

libedleak_probe_a_SOURCES = \
    probe/MemAlignProbe.cpp \
    probe/MemAllocProbe.cpp \
    probe/MemCallocProbe.cpp \
    probe/MemFreeProbe.cpp \
    probe/MemProbe.cpp \
    probe/MemReallocProbe.cpp \
    probe/Probe.cpp

libedleak_la_SOURCES =

libedleak_controller_a_CPPFLAGS = $(AM_CPPFLAGS)
libedleak_controller_a_CPPFLAGS += -DFW_DEFAULT_FILENAME=\"edleak.json\"
libedleak_controller_a_CPPFLAGS += -DFW_DEFAULT_POLL_PERIOD=30

libedleak_heap_a_CPPFLAGS = $(AM_CPPFLAGS)
libedleak_preload_a_CPPFLAGS = $(AM_CPPFLAGS)
libedleak_probe_a_CPPFLAGS = $(AM_CPPFLAGS)

libedleak_la_CXXFLAGS = -fPIC

libedleak_la_LIBADD = \
    $(top_builddir)/src/toolbox/libedkit-toolbox.a \
    $(builddir)/libedleak-controller.a \
    $(JSON_LIBS)

