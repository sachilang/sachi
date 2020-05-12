#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

m4_define(SACHI_VERSION, 0.1)
m4_include([m4/ax_code_coverage.m4])

AC_PREREQ([2.69])
AC_INIT([sachi], [SACHI_VERSION], [https://github.com/sachilang/sachi/issues])
AC_CONFIG_SRCDIR([sachi/main.cpp])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB
AX_CODE_COVERAGE

AC_LANG([C])

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([float.h limits.h locale.h stddef.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRTOD
AC_CHECK_FUNCS([localeconv memset strrchr])

AC_CONFIG_FILES([Makefile
                 external/cJSON/Makefile
                 sachi/Makefile
                 test/Makefile])
AC_OUTPUT
