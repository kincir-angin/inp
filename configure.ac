#
# InOS Package Manager 
#
# Author: Naufal Adriansyah
# Organization: Kincir Angin Studio
#

AC_INIT([inp], [0.01])

# We need at least autoconf version 2.71
AC_PREREQ([2.71])

AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([include/config.h])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror])
AC_CONFIG_FILES([Makefile])

# FLAGS
INCLUDE="-I${srcdir}/include"

AC_SUBST(INCLUDE)

AC_PROG_CC

AC_OUTPUT