check_PROGRAMS = utest_edkit_edleak

TESTS = $(check_PROGRAMS)

GLIB_SOURCE = \
    glib/TestSliceAllocProbe.cpp \
    glib/TestSliceFreeProbe.cpp

utest_edkit_edleak_SOURCES = \
    main.cpp \
    FakeAlloc.cpp \
    MemAlign1.cpp \
    TestExeContext.cpp \
    TestMemAlloc.cpp \
    mock/rtsym.cpp

utest_edkit_edleak_CXXFLAGS = -fPIC

utest_edkit_edleak_CPPFLAGS = -D_GNU_SOURCE \
    -I$(top_srcdir)/include/edkit/toolbox \
    -I$(top_srcdir)/src/edleak/probe \
    -I$(top_srcdir)/src/edleak/heap

utest_edkit_edleak_CPPFLAGS += $(CPPUTEST_CFLAGS)
utest_edkit_edleak_LDADD  = \
    $(CPPUTEST_LIBS) \
    $(top_srcdir)/src/edleak/libedleak-probe.a \
    $(top_srcdir)/src/edleak/libedleak-heap.a \
    $(top_srcdir)/src/toolbox/libedkit-toolbox.a

