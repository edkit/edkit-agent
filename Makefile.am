ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = bootstrap

SUBDIRS = src/toolbox src/edleak tools/leaker 
if HAVE_CPPUTEST
    SUBDIRS += \
        test/toolbox \
        test/edleak
endif
