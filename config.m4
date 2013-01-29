dnl $Id$
dnl config.m4 for extension tc

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(tc, for tc support,
Make sure that the comment is aligned:
[  --with-tc             Include tc support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(tc, whether to enable tc support,
dnl Make sure that the comment is aligned:
dnl [  --enable-tc           Enable tc support])

if test "$PHP_TC" != "no"; then
  dnl Write more examples of tests here...

  # --with-tc -> check with-path
  SEARCH_PATH="/usr/local /usr"     # you might want to change this
  SEARCH_FOR="/include/tchdb.h"  # you most likely want to change this
  if test -r $PHP_TC/$SEARCH_FOR; then # path given as parameter
    TC_DIR=$PHP_TC
  else # search default path list
    AC_MSG_CHECKING([for tc files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        TC_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi
  
  if test -z "$TC_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the tc distribution])
  fi

  dnl # --with-tc -> add include path
  PHP_ADD_INCLUDE($TC_DIR/include)

  dnl # --with-tc -> check for lib and symbol presence
  LIBNAME=tokyocabinet # you may want to change this
  LIBSYMBOL=tchdbnew # you most likely want to change this 

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $TC_DIR/lib, TC_SHARED_LIBADD)
    AC_DEFINE(HAVE_TCLIB,1,[ ])
  ],[
    AC_MSG_ERROR([wrong tc lib version or lib not found])
  ],[
    -L$TC_DIR/lib -lm -ldl
  ])
  
  PHP_SUBST(TC_SHARED_LIBADD)
  PHP_ADD_LIBRARY(stdc++, "", EXTERN_NAME_LIBADD)
  PHP_NEW_EXTENSION(tc, tc.c, $ext_shared)
fi
