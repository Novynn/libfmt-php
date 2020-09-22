dnl config.m4 for extension fmt

PHP_ARG_ENABLE(fmt, whether to enable fmt support,
[  --enable-fmt           Enable fmt support])

if test "$PHP_FMT" != "no"; then
  PHP_REQUIRE_CXX()
  PHP_NEW_EXTENSION(fmt, fmt.cc, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
  PHP_ADD_EXTENSION_DEP(spl)
  PHP_SUBST(FMT_SHARED_LIBADD)

  PHP_ADD_LIBRARY(stdc++,11,FMT_SHARED_LIBADD)

  PHP_FMT_DIR=$ext_srcdir/fmt/include
  if test -f $PHP_FMT_DIR/fmt/format.h; then
    PHP_ADD_INCLUDE([$PHP_FMT_DIR])
  else
    AC_MSG_ERROR([Could not find bundled libfmt])
  fi
fi
