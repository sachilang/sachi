check_PROGRAMS=test
test_LIBS=$(CODE_COVERAGE_LIBS)
test_SOURCES=$(top_srcdir)/sachi/module.c $(top_srcdir)/sachi/parser.c main.cpp
test_CPPFLAGS=-DCJSON_HIDE_SYMBOLS -DSACHI_HIDE_SYMBOLS -I$(top_srcdir)/external -I$(top_srcdir) -fPIC $(CODE_COVERAGE_CPPFLAGS) --coverage
test_CFLAGS=$(CODE_COVERAGE_CFLAGS)
test_CXXFLAGS=$(CODE_COVERAGE_CXXFLAGS)
test_LDADD=$(top_srcdir)/external/cJSON/libcjson.a $(CODE_COVERAGE_LDFLAGS)

TESTS=$(check_PROGRAMS)
