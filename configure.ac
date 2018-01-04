#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([darr], [0.0.1], [takuyamakino15@gmail.com])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([darr/da.cpp])
AC_CONFIG_HEADERS([config.h])

AC_PROG_LIBTOOL

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

DARR_DESCRIPTION='A Double Array implementation'
AC_SUBST(DARR_DESCRIPTION)
AC_ARG_WITH([pkgconfigdir],
	      [AS_HELP_STRING([--with-pkgconfigdir=DIR], [pkgconfig files])],
	        [pkgconfigdir=$withval],
		  [pkgconfigdir="${libdir}/pkgconfig"])
AC_SUBST([pkgconfigdir], [$pkgconfigdir])
AC_MSG_NOTICE([pkgconfig directory is ${pkgconfigdir}])

AC_ARG_WITH([gperf],
	      [AS_HELP_STRING([--with-gperf=DIR], [gperf install prefix])],
		[DARR_GPERF_INSTALL_PREFIX=$withval],
		  [DARR_GPERF_INSTALL_PREFIX="${libdir}"])
AC_MSG_NOTICE([!!! gperf directory is ${DARR_GPERF_INSTALL_PREFIX}])
AC_SUBST(DARR_GPERF_INSTALL_PREFIX)

# Checks for library functions.
AC_CHECK_FUNCS([mblen setlocale])

AC_ARG_ENABLE([debug],
	      [  --enable-debug   Turn on debugging],
	      [case "${enableval}" in
	       yes) debug=true ;;
	       no)  debug=false ;;
	       *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
       	       esac],
	      [debug=false])
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])

AC_CONFIG_FILES([
		 Makefile
		 darr/Makefile
		 example/Makefile
		 libdarr.pc
		 ])
AC_OUTPUT
