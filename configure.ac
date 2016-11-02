
#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([edkit-agent], [0.1.0])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/edleak/probe/MemAllocProbe.cpp])
AC_CONFIG_HEADERS([config.h])
AM_PROG_AR
LT_INIT

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
PKG_PROG_PKG_CONFIG

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h inttypes.h limits.h malloc.h stddef.h stdint.h stdlib.h string.h])

# check for libraries
AC_SEARCH_LIBS([pthread_create], [pthread], [], [
  AC_MSG_ERROR([unable to find the pthread_create function])
])

AC_SEARCH_LIBS([dlopen], [dl dld], [], [
  AC_MSG_ERROR([unable to find the dlopen() function])
])

PKG_CHECK_MODULES([cpputest], [cpputest >= 3.6],
      [have_cpputest=yes], [have_cpputest=no])

PKG_CHECK_MODULES([jsonc], [json-c >= 0.9],
      [have_json=yes], [
   AC_MSG_ERROR([
      You need to install or upgrade json-c library.
  ])
])

PKG_CHECK_MODULES([libunwind], [libunwind >= 1.1],
      [have_libunwind=yes], [have_libunwind=no])

PKG_CHECK_MODULES([glib], [glib-2.0 >= 2.32],
    [have_libglib=yes], [have_libglib=no])

AC_ARG_ENABLE([jemalloc],
    AS_HELP_STRING([--enable-jemalloc], [Enable jemalloc allocator]))

AS_IF([test "x$enable_jemalloc" = "xyes"], [
    PKG_CHECK_MODULES([jemalloc], [jemalloc >= 4.2],
        [have_jemalloc=yes], [have_jemalloc=no])
])

AC_ARG_WITH([jemalloc_prefix],
  [AS_HELP_STRING([--with-jemalloc-prefix=<prefix>], [Prefix of jemalloc])],
  [JEMALLOC_PREFIX="$with_jemalloc_prefix"],
  [JEMALLOC_PREFIX="je_"]
)

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

CXXFLAGS="$CXXFLAGS -fno-optimize-sibling-calls -fno-omit-frame-pointer"

AC_CONFIG_FILES([Makefile src/toolbox/Makefile src/edleak/Makefile tools/leaker/Makefile test/toolbox/Makefile test/edleak/Makefile include/edkit/toolbox/rtsym.h])
AM_CONDITIONAL([HAVE_CPPUTEST], [test "x${have_cpputest}" = xyes])
AM_CONDITIONAL([HAVE_LIBUNWIND], [test "x${have_libunwind}" = xyes])
AM_CONDITIONAL([HAVE_LIBGLIB], [test "x${have_libglib}" = xyes])

if test "x${have_libunwind}" = xyes; then
    AC_DEFINE(HAVE_LIBUNWIND,1, [libunwind is available])
fi

if test "x${have_libglib}" = xyes; then
    AC_DEFINE(HAVE_LIBGLIB,1, [libglib is available])
fi

if test "x${have_jemalloc}" = xyes; then
    AC_DEFINE(HAVE_JEMALLOC,1, [jemalloc is available])

    if test "x$JEMALLOC_PREFIX" != "x" ; then
      AC_DEFINE_UNQUOTED([JEMALLOC_PREFIX], ["$JEMALLOC_PREFIX"], [jemalloc prefix defined])
    fi
fi


MANGLE_SYMBOL([new], [void* operator new(std::size_t)], [MNGL_SYM_NEW])
MANGLE_SYMBOL([new], [void* operator new(std::size_t, const std::nothrow_t& tag)], [MNGL_SYM_NEW_NOTHROW])
MANGLE_SYMBOL([new], [void operator delete(void*)], [MNGL_SYM_DELETE])
MANGLE_SYMBOL([new], [void operator delete(void*, const std::nothrow_t& tag)], [MNGL_SYM_DELETE_NOTHROW])

AC_OUTPUT
echo "
edkit agents ($PACKAGE_NAME) version $PACKAGE_VERSION
Prefix.........: $prefix
Debug Build....: $debug
C++ Compiler...: $CXX
CFLAGS.........: $CFLAGS
CXXFLAGS.......: $CXXFLAGS
CPPFLAGS.......: $CPPFLAGS
Linker.........: $LD $LDFLAGS $LIBS"

if test "x${have_libglib}" = xyes; then
   echo "glib support...: Enabled"
else
   echo "glib support...: Disabled"
fi

if test "x${have_cpputest}" = xyes; then
   echo "CppUTest.......: Enabled"
else
   echo "CppUTest.......: Disabled"
fi

if test "x${have_libunwind}" = xyes; then
   echo "libunwind......: Enabled"
else
   echo "libunwind......: Disabled"
fi

if test "x${have_jemalloc}" = xyes; then
   echo "jemalloc.......: Enabled"
   echo "jemalloc prefix: $JEMALLOC_PREFIX"
else
   echo "jemalloc.......: Disabled"
fi
