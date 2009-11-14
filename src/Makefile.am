ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = test
lib_LTLIBRARIES = libgettid.la
include_HEADERS = libgettid.h

test_SOURCES = test.c libgettid.h
test_LDADD = -lgettid
libgettid_la_SOURCES = libgettid.c libgettid.h
libgettid_la_LDFLAGS = -version 0:0:0
libgettid_la_LIBADD = $(DLLIB)



