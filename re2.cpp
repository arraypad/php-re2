/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2011 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Arpad Ray <arpad@php.net>                                    |
  +----------------------------------------------------------------------+
*/

extern "C" {
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
}

#include "php_re2.h"

#include <re2/re2.h>
#include <string>


/* {{{ arg info */
ZEND_BEGIN_ARG_INFO_EX(arginfo_re2_match, 0, 0, 2)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, subject)
	ZEND_ARG_INFO(0, argc)
	ZEND_ARG_INFO(1, matches)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_re2_match_all, 0, 0, 4)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, subject)
	ZEND_ARG_INFO(0, argc)
	ZEND_ARG_INFO(1, matches)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_re2_replace, 0, 0, 3)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, subject)
	ZEND_ARG_INFO(0, replace)
	ZEND_ARG_INFO(1, count)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_re2_compile, 0, 0, 1)
	ZEND_ARG_INFO(0, pattern)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ re2_functions[]
 */
const zend_function_entry re2_functions[] = {
	PHP_FE(re2_match, arginfo_re2_match)
	PHP_FE(re2_match_all, arginfo_re2_match_all)
	PHP_FE(re2_replace, arginfo_re2_replace)
	PHP_FE(re2_quote, NULL)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ RE2 class */
zend_class_entry *php_re2_class_entry;
#define PHP_RE2_CLASS_NAME "RE2"

static zend_function_entry re2_class_functions[] = {
	PHP_ME(RE2, __construct, arginfo_re2_compile, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ RE2 object handlers */
zend_object_handlers re2_object_handlers;

struct re2_object {
	zend_object std;
	RE2 *re2;
};

void re2_free_storage(void *object TSRMLS_DC)
{
	re2_object *obj = (re2_object *)object;
	delete obj->re2;

	zend_hash_destroy(obj->std.properties);
	FREE_HASHTABLE(obj->std.properties);

	efree(obj);
}

zend_object_value re2_create_handler(zend_class_entry *type TSRMLS_DC)
{
	zval *tmp;
	zend_object_value retval;

	re2_object *obj = (re2_object *)emalloc(sizeof(re2_object));
	memset(obj, 0, sizeof(re2_object));
	obj->std.ce = type;

	ALLOC_HASHTABLE(obj->std.properties);
	zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_copy(obj->std.properties, &type->default_properties, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(obj, NULL, re2_free_storage, NULL TSRMLS_CC);
	retval.handlers = &re2_object_handlers;
	return retval;
}
/* }}} */

#define RE2_GET_PATTERN \
	if (Z_TYPE_P(pattern) == IS_STRING) { \
		re2 = new RE2(Z_STRVAL_P(pattern)); \
	} else if (Z_TYPE_P(pattern) == IS_OBJECT && instanceof_function(Z_OBJCE_P(pattern), php_re2_class_entry TSRMLS_CC)) { \
		re2_object *obj = (re2_object *)zend_object_store_get_object(pattern TSRMLS_CC); \
		re2 = obj->re2; \
	} else { \
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Pattern must be a string or an RE2 object"); \
		RETURN_FALSE; \
	}

/* {{{ PHP_FUNCTION(re2_match) */
PHP_FUNCTION(re2_match)
{
	char *subject;
	std::string subject_str;
	int subject_len;
	long argc;
	zval *pattern = NULL, *matches = NULL;
	RE2 *re2;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zs|lz", &pattern, &subject, &subject_len, &argc, &matches) == FAILURE) {
		RETURN_FALSE;
	}

	if (ZEND_NUM_ARGS() == 3) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Number of subpatterns argument passed but no matches argument");
		RETURN_FALSE;
	}

	RE2_GET_PATTERN;

	subject_str = std::string(subject);

	if (ZEND_NUM_ARGS() > 2) {
		int i;
		std::string str[argc];
		RE2::Arg argv[argc];
		RE2::Arg *args[argc];

		for (i = 0; i < argc; i++) {
			argv[i] = &str[i];
			args[i] = &argv[i];
		}

		bool match = RE2::PartialMatchN(subject_str, *re2, args, argc);

		if (match) {
			if (matches != NULL) {
				zval_dtor(matches);
			}

			array_init_size(matches, argc);
			for (i = 0; i < argc; i++) {
				add_next_index_string(matches, str[i].c_str(), 1);
			}
			RETURN_TRUE;
		} else {
			RETURN_FALSE;
		}
	} else {
		bool match = RE2::PartialMatch(subject_str, *re2);
		if (match) {
			RETURN_TRUE;
		} else {
			RETURN_FALSE;
		}
	}
}
/* }}} */

/* {{{ PHP_FUNCTION(re2_match_all) */
PHP_FUNCTION(re2_match_all)
{
	char *subject;
	std::string subject_str;
	re2::StringPiece subject_piece;
	int subject_len, i;
	long argc;
	zval *pattern = NULL, *matches = NULL, *piece_matches = NULL;
	bool did_match = false, was_empty = false;
	RE2 *re2;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zslz", &pattern, &subject, &subject_len, &argc, &matches) == FAILURE) {
		RETURN_FALSE;
	}

	RE2_GET_PATTERN;

	subject_str = std::string(subject);
	subject_piece = re2::StringPiece(subject_str);

	std::string str[argc];
	RE2::Arg argv[argc];
	RE2::Arg *args[argc];

	for (i = 0; i < argc; i++) {
		argv[i] = &str[i];
		args[i] = &argv[i];
	}

	while (RE2::FindAndConsumeN(&subject_piece, *re2, args, argc)) {
		if (!did_match) {
			if (matches != NULL) {
				zval_dtor(matches);
			}
			array_init(matches);
			did_match = true;
		}

		if (subject_piece.empty()) {
			if (was_empty) {
				/* matched zero-length regex, exit to avoid infinite loop */
				break;
			}
			was_empty = true;
		}
		

		MAKE_STD_ZVAL(piece_matches);
		array_init_size(piece_matches, argc);
		for (i = 0; i < argc; i++) {
			add_next_index_string(piece_matches, str[i].c_str(), 1);
		}
		add_next_index_zval(matches, piece_matches);
		piece_matches = NULL;
	}

	if (did_match) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}
/* }}} */

/* {{{ PHP_FUNCTION(re2_replace) */
PHP_FUNCTION(re2_replace)
{
	char *subject, *replace;
	std::string subject_str, pattern_str, replace_str;
	int subject_len, replace_len, i;
	long count;
	zval *pattern, *count_zv, *out;
	RE2 *re2;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zss|z", &pattern, &replace, &replace_len, &subject, &subject_len, &count_zv) == FAILURE) {
		RETURN_FALSE;
	}

	RE2_GET_PATTERN;

	subject_str = std::string(subject);
	replace_str = std::string(replace);

	count = RE2::GlobalReplace(&subject_str, *re2, replace_str);

	if (ZEND_NUM_ARGS() == 4) {
		ZVAL_LONG(count_zv, count);
	}

	RETVAL_STRINGL(subject_str.c_str(), subject_str.length(), 1);
}
/* }}} */

/* {{{ PHP_FUNCTION(re2_quote) */
PHP_FUNCTION(re2_quote)
{
	char *subject;
	std::string subject_str, out_str;
	int subject_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &subject, &subject_len) == FAILURE) {
		RETURN_FALSE;
	}

	subject_str = std::string(subject);
	out_str = RE2::QuoteMeta(subject);
	RETVAL_STRINGL(out_str.c_str(), out_str.length(), 1);
}
/* }}} */

/* {{{ PHP_METHOD(RE2, __construct) */
PHP_METHOD(RE2, __construct)
{
	char *pattern;
	int pattern_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &pattern, &pattern_len) == FAILURE) {
		RETURN_NULL();
	}

	RE2 *re2_obj = new RE2(pattern);
	re2_object *obj = (re2_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	obj->re2 = re2_obj;
}
/* }}} */

/* {{{ re2_module_entry
 */
zend_module_entry re2_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"re2",
	re2_functions,
	PHP_MINIT(re2),
	PHP_MSHUTDOWN(re2),
	PHP_RINIT(re2),
	PHP_RSHUTDOWN(re2),
	PHP_MINFO(re2),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_RE2_EXTVER,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_RE2
extern "C" {
ZEND_GET_MODULE(re2)
}
#endif

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(re2)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, PHP_RE2_CLASS_NAME, re2_class_functions);
	php_re2_class_entry = zend_register_internal_class(&ce TSRMLS_CC);
	php_re2_class_entry->create_object = re2_create_handler;
	memcpy(&re2_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	re2_object_handlers.clone_obj = NULL;
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(re2)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(re2)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(re2)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(re2)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "re2 support", "enabled");
	php_info_print_table_row(2, "re2 version", PHP_RE2_EXTVER);
	php_info_print_table_end();
}
/* }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
