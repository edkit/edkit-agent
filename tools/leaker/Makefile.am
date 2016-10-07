AM_CPPFLAGS = $(LIBGLIB_CFLAGS)

noinst_PROGRAMS = leaker


leaker_SOURCES = \
    main.cpp

leaker_LDFLAGS = -Wl,-export-dynamic

if HAVE_LIBGLIB
    leaker_LDFLAGS += $(LIBGLIB_LIBS)
endif
