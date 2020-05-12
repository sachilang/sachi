bin_PROGRAMS=sachi
sachi_SOURCES=module.c parser.c main.cpp
sachi_CPPFLAGS=-DSACHI_HIDE_SYMBOLS -I$(srcdir)/../external -fPIC
sachi_LDADD=$(srcdir)/../external/cJSON/libcjson.a
