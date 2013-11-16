#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

m4_include(m4/acx_pthread.m4)

AC_PREREQ([2.68])
AC_INIT([libcds2], [2.0], [fclaude@gmail.com])
AC_CONFIG_SRCDIR([src/basic/array.cpp])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([build-aux/config.h])
AM_INIT_AUTOMAKE([1.9 subdir-objects])
LT_INIT
AC_SUBST([LIBTOOL_DEPS])

# Checks for programs.
# If we want to explicitly set this to a different optimization level, this
# is where we do it.  But do note this is not conventionally approved of.
# ${CXXFLAGS="-O3"}
AC_PROG_CXX
AC_PROG_CC
AC_LANG([C++])
AC_PROG_INSTALL
AC_PROG_CPP

# Checks for libraries.
ACX_PTHREAD([],
            [AC_MSG_FAILURE([libcds2 requires pthreads.])])

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdint.h stdlib.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([gettimeofday select])

AC_CONFIG_FILES([Makefile lib/Makefile])
AC_CONFIG_SUBDIRS([deps/gtest-1.6.0])
AC_OUTPUT
