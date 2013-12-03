#                                               -*- Autoconf -*-
# To rebuild the configure script and Makefile.in files (as well as some
# supporting files mainly in build-aux/ and m4/) run the following
# command:
#     autoreconf -vif --no-recursive
# The last flag is there to prevent autoconf from trying to rebuild
# the autoconf files in dep/gflags-1.6.0.

m4_include(m4/acx_pthread.m4)

AC_PREREQ([2.68])
AC_INIT([libcds2], [2.0], [fclaude@gmail.com], [cds2],
        [https://github.com/fclaude/libcds2])
AC_CONFIG_SRCDIR([src/basic/array.cpp])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([build-aux/config.h])
AM_INIT_AUTOMAKE([1.9 subdir-objects])
AM_SILENT_RULES([yes])
LT_INIT([shared static])
AC_SUBST([LIBTOOL_DEPS])
AC_SUBST([CDS_SO_VERSION], [2:0:0])
AC_SUBST([CDS_API_VERSION], [2])

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

AC_CONFIG_FILES([Makefile lib/Makefile tests/Makefile include/Makefile
                 cds${CDS_API_VERSION}.pc:cds.pc.in])
AC_CONFIG_SUBDIRS([dep/gtest-1.6.0])
AC_OUTPUT
