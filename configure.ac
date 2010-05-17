AC_INIT([libgettid], [0.1], [g.bagnoli@asidev.com])
AC_COPYRIGHT([Copyright (C) 2009 Giacomo Bagnoli <g.bagnoli@asidev.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror foreign dist-bzip2])
AC_PROG_CC
AC_PROG_LIBTOOL
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/libgettid.c])
AC_HEADER_STDC
AC_CHECK_HEADERS([linux/unistd.h sys/syscall.h])
AC_CHECK_LIB([pthread], [pthread_create])
AC_CHECK_LIB([dl], [dlsym], [DLLIB=-ldl])
AC_SUBST([DLLIB])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Doxyfile Makefile src/Makefile libgettid.pc])
AC_OUTPUT

