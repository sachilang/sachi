check_PROGRAMS=test
test_SOURCES=$(top_srcdir)/sachi/module.c $(top_srcdir)/sachi/parser.c main.cpp
test_CPPFLAGS=-DCJSON_HIDE_SYMBOLS -DSACHI_HIDE_SYMBOLS -I$(top_srcdir)/external -I$(top_srcdir) -fPIC
test_LDADD=$(top_srcdir)/external/cJSON/libcjson.a
