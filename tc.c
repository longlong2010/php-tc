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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_tc.h"
#include <tcutil.h>
#include <tchdb.h>

/* If you declare any globals in php_tc.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(tc)
*/

/* True global resources - no need for thread safety here */
static int le_tch;
static zend_class_entry* tch_entry_ptr;

typedef struct _php_tchdb {
    TCHDB* hdb;
} php_tchdb;
/* {{{ tc_functions[]
 *
 * Every user visible function must have an entry in tc_functions[].
 */
zend_function_entry tc_functions[] = {
    PHP_FE_END
};

zend_function_entry tch_class_methods[] = {
    PHP_ME(tch, __construct, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(tch, open, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(tch, close, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(tch, get, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(tch, put, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(tch, out, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(tch, iterinit, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(tch, iternext, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(tch, fsiz, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(tch, rnum, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(tch, vsiz, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(tch, setcache, NULL, ZEND_ACC_PUBLIC)
    PHP_FE_END
};
/* }}} */

/* {{{ tc_module_entry
 */
zend_module_entry tc_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"tc",
	tc_functions,
	PHP_MINIT(tc),
	PHP_MSHUTDOWN(tc),
	PHP_RINIT(tc),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(tc),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(tc),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_TC
ZEND_GET_MODULE(tc)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("tc.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_tc_globals, tc_globals)
    STD_PHP_INI_ENTRY("tc.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_tc_globals, tc_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_tc_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_tc_init_globals(zend_tc_globals *tc_globals)
{
	tc_globals->global_value = 0;
	tc_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */

static void _tchdbdel(zend_rsrc_list_entry* rsrc TSRMLS_DC) {
    php_tchdb* hdb = (php_tchdb*) rsrc->ptr;
    tchdbdel(hdb->hdb);
    efree(hdb);
}

PHP_MINIT_FUNCTION(tc)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
    zend_class_entry tch_entry;
    INIT_CLASS_ENTRY(tch_entry, "tchdb", tch_class_methods);
    tch_entry_ptr = zend_register_internal_class(&tch_entry TSRMLS_CC);
    zend_declare_property_null(tch_entry_ptr, ZEND_STRL("_hdb"), ZEND_ACC_PROTECTED TSRMLS_CC);

    le_tch = zend_register_list_destructors_ex(_tchdbdel, NULL, "tchdb", module_number);

	REGISTER_LONG_CONSTANT("HDBOCREAT", HDBOCREAT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("HDBOREADER", HDBOREADER, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("HDBOWRITER", HDBOWRITER, CONST_CS | CONST_PERSISTENT);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(tc)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(tc)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(tc)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(tc)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "tc support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_tc_compiled(string arg)
   Return a string to confirm that the module is compiled in */

PHP_METHOD(tch, __construct) {
    zval* self = getThis();
    zval* _hdb;
    MAKE_STD_ZVAL(_hdb);
    php_tchdb* hdb = (php_tchdb*) emalloc(sizeof(php_tchdb));
    hdb->hdb = tchdbnew();
    ZEND_REGISTER_RESOURCE(_hdb, hdb, le_tch); 
    zend_update_property(tch_entry_ptr, self, ZEND_STRL("_hdb"),  _hdb TSRMLS_CC);
}

PHP_METHOD(tch, open) {
    zval* self = getThis();
    const char* path;
    int path_len; 
    long mode;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl", &path, &path_len, &mode) == FAILURE) {
		return;
	}
    zval* _hdb = zend_read_property(tch_entry_ptr, self, ZEND_STRL("_hdb"), 1 TSRMLS_CC);
    php_tchdb* hdb;
    ZEND_FETCH_RESOURCE(hdb, php_tchdb*, &_hdb, -1, "tchdb", le_tch);
    bool rc = tchdbopen(hdb->hdb, path, mode);
    if (rc) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}

PHP_METHOD(tch, close) {
    zval* self = getThis();
    zval* _hdb = zend_read_property(tch_entry_ptr, self, ZEND_STRL("_hdb"), 1 TSRMLS_CC);
    php_tchdb* hdb;
    ZEND_FETCH_RESOURCE(hdb, php_tchdb*, &_hdb, -1, "tchdb", le_tch);
    bool rc = tchdbclose(hdb->hdb);
    if (rc) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}

PHP_METHOD(tch, get) {
    zval* self = getThis();
    const char* kstr;
    int kstr_len;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &kstr, &kstr_len) == FAILURE) {
		return;
	}
    zval* _hdb = zend_read_property(tch_entry_ptr, self, ZEND_STRL("_hdb"), 1 TSRMLS_CC);
    php_tchdb* hdb;
    ZEND_FETCH_RESOURCE(hdb, php_tchdb*, &_hdb, -1, "tchdb", le_tch);
    char* vstr = tchdbget2(hdb->hdb, kstr);
    if (vstr == NULL) {
        RETURN_FALSE;
    }
    char* evstr = estrdup(vstr);
    free(vstr);
    RETURN_STRING(evstr, 0);
}

PHP_METHOD(tch, put) {
    zval* self = getThis();
    const char* kstr;
    int kstr_len;
    const char* vstr;
    int vstr_len;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &kstr, &kstr_len, &vstr, &vstr_len) == FAILURE) {
		return;
	}
    zval* _hdb = zend_read_property(tch_entry_ptr, self, ZEND_STRL("_hdb"), 1 TSRMLS_CC);
    php_tchdb* hdb;
    ZEND_FETCH_RESOURCE(hdb, php_tchdb*, &_hdb, -1, "tchdb", le_tch);
    bool rc = tchdbput2(hdb->hdb, kstr, vstr);
    if (rc) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}

PHP_METHOD(tch, out) {
    zval* self = getThis();
    const char* kstr;
    int kstr_len;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &kstr, &kstr_len) == FAILURE) {
		return;
	}
    zval* _hdb = zend_read_property(tch_entry_ptr, self, ZEND_STRL("_hdb"), 1 TSRMLS_CC);
    php_tchdb* hdb;
    ZEND_FETCH_RESOURCE(hdb, php_tchdb*, &_hdb, -1, "tchdb", le_tch);
    bool rc = tchdbout2(hdb->hdb, kstr);
    if (rc) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}

PHP_METHOD(tch, iterinit) {
    zval* self = getThis();
    zval* _hdb = zend_read_property(tch_entry_ptr, self, ZEND_STRL("_hdb"), 1 TSRMLS_CC);
    php_tchdb* hdb;
    ZEND_FETCH_RESOURCE(hdb, php_tchdb*, &_hdb, -1, "tchdb", le_tch);
    bool rc = tchdbiterinit(hdb->hdb);
    if (rc) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}

PHP_METHOD(tch, iternext) {
    zval* self = getThis();
    zval* _hdb = zend_read_property(tch_entry_ptr, self, ZEND_STRL("_hdb"), 1 TSRMLS_CC);
    php_tchdb* hdb;
    ZEND_FETCH_RESOURCE(hdb, php_tchdb*, &_hdb, -1, "tchdb", le_tch);
    char* kstr = tchdbiternext2(hdb->hdb);
    if (kstr == NULL) {
        RETURN_FALSE;
    }
    char* ekstr = estrdup(kstr);
    free(kstr);
    RETURN_STRING(ekstr, 0);
}

PHP_METHOD(tch, vsiz) {
    zval* self = getThis();
    const char* kstr;
    int kstr_len;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &kstr, &kstr_len) == FAILURE) {
		return;
	}
    zval* _hdb = zend_read_property(tch_entry_ptr, self, ZEND_STRL("_hdb"), 1 TSRMLS_CC);
    php_tchdb* hdb;
    ZEND_FETCH_RESOURCE(hdb, php_tchdb*, &_hdb, -1, "tchdb", le_tch);
    int siz = tchdbvsiz2(hdb->hdb, kstr);
    RETURN_LONG(siz);
}

PHP_METHOD(tch, fsiz) {
    zval* self = getThis();
    zval* _hdb = zend_read_property(tch_entry_ptr, self, ZEND_STRL("_hdb"), 1 TSRMLS_CC);
    php_tchdb* hdb;
    ZEND_FETCH_RESOURCE(hdb, php_tchdb*, &_hdb, -1, "tchdb", le_tch);
    uint64_t fsiz = tchdbfsiz(hdb->hdb);
    RETURN_LONG(fsiz);
}

PHP_METHOD(tch, rnum) {
    zval* self = getThis();
    zval* _hdb = zend_read_property(tch_entry_ptr, self, ZEND_STRL("_hdb"), 1 TSRMLS_CC);
    php_tchdb* hdb;
    ZEND_FETCH_RESOURCE(hdb, php_tchdb*, &_hdb, -1, "tchdb", le_tch);
    uint64_t rnum = tchdbrnum(hdb->hdb);
    RETURN_LONG(rnum);
}

PHP_METHOD(tch, setcache) {
    zval* self = getThis();
    long rcnum;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &rcnum) == FAILURE) {
		return;
	}
    zval* _hdb = zend_read_property(tch_entry_ptr, self, ZEND_STRL("_hdb"), 1 TSRMLS_CC);
    php_tchdb* hdb;
    ZEND_FETCH_RESOURCE(hdb, php_tchdb*, &_hdb, -1, "tchdb", le_tch);
    bool rc = tchdbsetcache(hdb->hdb, rcnum);
    if (rc) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
