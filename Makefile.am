SUBDIRS = \
          external/cJSON \
          sachi \
          test

TESTS=$(check_PROGRAMS)

@CODE_COVERAGE_RULES@
clean-local: code-coverage-clean
distclean-local: code-coverage-dist-clean
