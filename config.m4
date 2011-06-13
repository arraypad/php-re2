dnl config.m4 for extension re2

PHP_ARG_WITH(re2, for re2 support,
[  --with-re2             Include re2 support])

if test "$PHP_RE2" != "no"; then
  SEARCH_PATH="/usr/local /usr"
  SEARCH_FOR="/include/re2/re2.h"
  if test -r $PHP_RE2/$SEARCH_FOR; then
    RE2_DIR=$PHP_RE2
  else
    AC_MSG_CHECKING([for re2 files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        RE2_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi
  if test -z "$RE2_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the re2 distribution])
  fi

  PHP_ADD_INCLUDE($RE2_DIR/include/re2)
  PHP_ADD_LIBRARY(re2)

  PHP_REQUIRE_CXX()
  PHP_SUBST(RE2_SHARED_LIBADD)
  PHP_ADD_LIBRARY(stdc++, 1, RE2_SHARED_LIBADD)
  PHP_NEW_EXTENSION(re2, re2.cpp, $ext_shared)
fi
