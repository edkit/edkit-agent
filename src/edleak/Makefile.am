AM_CPPFLAGS = -D_GNU_SOURCE -I$(top_srcdir)/include/edkit/toolbox -I$(top_builddir)/include/edkit/toolbox -I$(srcdir) -I$(srcdir)/heap -I$(srcdir)/probe
AM_CPPFLAGS += $(LIBGLIB_CFLAGS) $(JEMALLOC_CFLAGS)
AM_CXXFLAGS = -fPIC

noinst_LTLIBRARIES = \
    libedleak-controller.la \
    libedleak-heap.la \
    libedleak-preload.la \
    libedleak-probe.la

lib_LTLIBRARIES = libedleak.la

libedleak_controller_la_SOURCES = \
    controller/ContextUtils.cpp \
    controller/Controllers.cpp \
    controller/FileWriter.cpp \
    controller/WebService.cpp \
    controller/WsMethodSlice.cpp \
    controller/WsMethodStackWatchAdd.cpp

libedleak_heap_la_SOURCES = \
    heap/ContextCallStackList.cpp \
    heap/ExeContext.cpp \
    heap/MemHeap.cpp

libedleak_preload_la_SOURCES = \
    preload/Posix.cpp

if HAVE_LIBGLIB
    libedleak_preload_la_SOURCES += preload/Glib.cpp
endif

libedleak_probe_la_SOURCES = \
    probe/MemAlignProbe.cpp \
    probe/MemAllocProbe.cpp \
    probe/MemCallocProbe.cpp \
    probe/MemFreeProbe.cpp \
    probe/MemProbe.cpp \
    probe/MemReallocProbe.cpp \
    probe/Probe.cpp

if HAVE_LIBGLIB
    libedleak_probe_la_SOURCES += \
        probe/glib/SliceAllocProbe.cpp \
        probe/glib/SliceFreeProbe.cpp
endif


libedleak_la_SOURCES = \
    PosixSymbol.cpp

libedleak_controller_la_CPPFLAGS = $(AM_CPPFLAGS)
libedleak_controller_la_CPPFLAGS += -DFW_DEFAULT_FILENAME=\"edleak.json\"
libedleak_controller_la_CPPFLAGS += -DFW_DEFAULT_POLL_PERIOD=30

libedleak_heap_la_CPPFLAGS = $(AM_CPPFLAGS)
libedleak_preload_la_CPPFLAGS = $(AM_CPPFLAGS)
libedleak_probe_la_CPPFLAGS = $(AM_CPPFLAGS)

libedleak_la_CPPFLAGS = $(AM_CPPFLAGS)
libedleak_la_CXXFLAGS = -fPIC
libedleak_la_LDFLAGS = -lstdc++

libedleak_la_LIBADD = \
    $(top_builddir)/src/toolbox/libedkit-toolbox.la \
    $(builddir)/libedleak-controller.la \
    $(builddir)/libedleak-heap.la \
    $(builddir)/libedleak-preload.la \
    $(builddir)/libedleak-probe.la \
    $(JSONC_LIBS)

if HAVE_LIBGLIB
    libedleak_la_LIBADD += $(LIBGLIB_LIBS)
endif

if HAVE_LIBUNWIND
    libedleak_la_LIBADD += $(LIBUNWIND_LIBS)
endif

if HAVE_JEMALLOC
    libedleak_la_LIBADD += $(JEMALLOC_LIBS)
endif
