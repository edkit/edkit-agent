
#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([edkit-agent], [0.1.0])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/edleak/probe/MemAllocProbe.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h inttypes.h limits.h malloc.h stddef.h stdint.h stdlib.h string.h])

# check for libraries
AX_PTHREAD
PKG_CHECK_MODULES([CPPUTEST], [cpputest >= 3.6], 
      [have_cpputest=yes], [have_cpputest=no])

PKG_CHECK_MODULES([JSON], [json >= 0.9],
      [have_json=yes], [
   AC_MSG_ERROR([
      You need to install or upgrade json-c library.
  ])
])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

AC_CONFIG_FILES([Makefile src/toolbox/Makefile src/edleak/Makefile test/Makefile])
AM_CONDITIONAL([COND_CPPUTEST], [test "x${have_cpputest}" = xyes])
AC_OUTPUT

echo "
aidl compiler ($PACKAGE_NAME) version $PACKAGE_VERSION
Prefix.........: $prefix
Debug Build....: $debug
C++ Compiler...: $CXX $CXXFLAGS $CPPFLAGS
Linker.........: $LD $LDFLAGS $LIBS"

if test "x${have_cpputest}" = xyes; then
   echo "CppUTest.......: $CPPUTEST_LIBS"
else
   echo "CppUTest.......: Disabled"
fi
