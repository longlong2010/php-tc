/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2007 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header 226204 2007-01-01 19:32:10Z iliaa $ */

#ifndef PHP_TC_H
#define PHP_TC_H

extern zend_module_entry tc_module_entry;
#define phpext_tc_ptr &tc_module_entry

#ifdef PHP_WIN32
#define PHP_TC_API __declspec(dllexport)
#else
#define PHP_TC_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(tc);
PHP_MSHUTDOWN_FUNCTION(tc);
PHP_RINIT_FUNCTION(tc);
PHP_RSHUTDOWN_FUNCTION(tc);
PHP_MINFO_FUNCTION(tc);
PHP_METHOD(tch, __construct);
PHP_METHOD(tch, open);
PHP_METHOD(tch, close);
PHP_METHOD(tch, get);
PHP_METHOD(tch, put);
PHP_METHOD(tch, out);
PHP_METHOD(tch, iterinit);
PHP_METHOD(tch, iternext);
PHP_METHOD(tch, fsiz);
PHP_METHOD(tch, rnum);
PHP_METHOD(tch, vsiz);
PHP_METHOD(tch, setcache);
/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(tc)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(tc)
*/

/* In every utility function you add that needs to use variables 
   in php_tc_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as TC_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define TC_G(v) TSRMG(tc_globals_id, zend_tc_globals *, v)
#else
#define TC_G(v) (tc_globals.v)
#endif

#endif	/* PHP_TC_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
