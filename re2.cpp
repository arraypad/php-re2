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
/* }}} */

/* {{{ re2_functions[]
 */
const zend_function_entry re2_functions[] = {
	PHP_FE(re2_match, arginfo_re2_match)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ */
PHP_FUNCTION(re2_match)
{
	char *subject, *pattern;
	std::string subject_str, pattern_str;
	int subject_len, pattern_len;
	long argc;
	zval *matches = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|lz", &pattern, &pattern_len, &subject, &subject_len, &argc, &matches) == FAILURE) {
		RETURN_FALSE;
	}

	subject_str = std::string(subject);
	pattern_str = std::string(pattern);

	if (ZEND_NUM_ARGS() > 2) {
		int i;
		std::string str[argc];
		RE2::Arg argv[argc];
		RE2::Arg *args[argc];

		for (i = 0; i < argc; i++) {
			argv[i] = &str[i];
			args[i] = &argv[i];
		}

		bool match = RE2::PartialMatchN(subject_str, pattern_str, args, argc);

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
		bool match = RE2::PartialMatch(subject_str, pattern_str);
		if (match) {
			RETURN_TRUE;
		} else {
			RETURN_FALSE;
		}
	}
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
