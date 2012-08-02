#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([libilbc], [1.1.1], [])
AC_CONFIG_AUX_DIR(.)
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([tar-ustar dist-bzip2 foreign])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Setup for libtool
AC_PROG_LIBTOOL

# Checks for programs.
AC_PROG_CC

DOLT

AC_SUBST(LIBTOOL_DEPS)

# soname version to use
# goes by ‘current[:revision[:age]]’ with the soname ending up as
# current.age.revision.
LIBILBC_VERSION=0:1:0
AC_SUBST(LIBILBC_VERSION)

AC_CONFIG_FILES([Makefile
                 libilbc.pc])
AC_OUTPUT
