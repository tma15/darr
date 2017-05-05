#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([darr], [0.0.1], [takuyamakino15@gmail.com])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([darr/da.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.
AC_CHECK_FUNCS([mblen setlocale])

AC_CONFIG_FILES([
		 Makefile
		 darr/Makefile
		 ])
AC_OUTPUT
