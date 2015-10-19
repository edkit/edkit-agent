check_PROGRAMS = utest_edkit_toolbox

TESTS = $(check_PROGRAMS)

utest_edkit_toolbox_SOURCES = \
    main.cpp \
    AList1.cpp \
    AListIterator1.cpp \
    CallStack.cpp \
    DynObject1.cpp \
    HttpdRequest1.cpp \
    JsonFlattener1.cpp \
    String1.cpp

utest_edkit_toolbox_LDFLAGS = -rdynamic
utest_edkit_toolbox_CXXFLAGS = -fPIC -fPIE

utest_edkit_toolbox_CPPFLAGS = -D_GNU_SOURCE -I$(top_srcdir)/include/edkit/toolbox
utest_edkit_toolbox_CPPFLAGS += $(CPPUTEST_CFLAGS)
utest_edkit_toolbox_LDADD  = $(CPPUTEST_LIBS) $(top_srcdir)/src/toolbox/libedkit-toolbox.a
utest_edkit_toolbox_LDADD += $(JSON_LIBS)

