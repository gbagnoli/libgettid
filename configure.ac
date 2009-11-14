AC_INIT([libgettid], [0.1], [g.bagnoli@asidev.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AC_CONFIG_SRCDIR([src/libgettid.c])
AC_CHECK_LIB([pthread], [pthread_create])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT

