dnl config.m4 for extension re2

PHP_ARG_WITH(re2, for re2 support,
[  --with-re2             Include re2 support])

if test "$PHP_RE2" != "no"; then
  SEARCH_PATH="$PHP_RE2 /usr/local /usr"
  SEARCH_INC="include/re2/re2.h"
  SEARCH_LIB="lib/libre2.so"
  AC_MSG_CHECKING([for re2 files])
  for i in $SEARCH_PATH ; do
    if test -r $i/$SEARCH_INC && test -r $i/$SEARCH_LIB; then
      RE2_DIR=$i
      AC_MSG_RESULT(found in $i)
    fi
  done
  if test -z "$RE2_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please ensure the re2 headers and shared library are installed])
  fi

  PHP_ADD_INCLUDE($RE2_DIR/include)

  PHP_REQUIRE_CXX()

  PHP_SUBST(RE2_SHARED_LIBADD)
  PHP_ADD_LIBRARY(stdc++, 1, RE2_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(re2, $RE2_DIR/lib, RE2_SHARED_LIBADD)

  PHP_NEW_EXTENSION(re2, re2.cpp, $ext_shared,,,1)
fi
