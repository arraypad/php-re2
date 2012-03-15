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
  | Author: Leif Jackson <ljackson@jjcons.com>                           |
  +----------------------------------------------------------------------+
*/

#ifndef PHP_RE2_H
#define PHP_RE2_H

#define PHP_RE2_EXTVER "0.0.2-dev"

#ifdef __cplusplus
extern "C" {
#endif

extern zend_module_entry re2_module_entry;
#define phpext_re2_ptr &re2_module_entry

#ifdef PHP_WIN32
#	define PHP_RE2_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_RE2_API __attribute__ ((visibility("default")))
#else
#	define PHP_RE2_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_FUNCTION(re2_match);
PHP_FUNCTION(re2_match_all);
PHP_FUNCTION(re2_replace);
PHP_FUNCTION(re2_replace_callback);
PHP_FUNCTION(re2_filter);
PHP_FUNCTION(re2_grep);
PHP_FUNCTION(re2_split);
PHP_FUNCTION(re2_quote);

PHP_METHOD(RE2, __construct);
PHP_METHOD(RE2, getOptions);
PHP_METHOD(RE2, getPattern);

PHP_METHOD(Re2Options, __construct);
PHP_METHOD(Re2Options, getEncoding);
PHP_METHOD(Re2Options, getMaxMem);
PHP_METHOD(Re2Options, getPosixSyntax);
PHP_METHOD(Re2Options, getLongestMatch);
PHP_METHOD(Re2Options, getLogErrors);
PHP_METHOD(Re2Options, getLiteral);
PHP_METHOD(Re2Options, getNeverNl);
PHP_METHOD(Re2Options, getCaseSensitive);
PHP_METHOD(Re2Options, getPerlClasses);
PHP_METHOD(Re2Options, getWordBoundary);
PHP_METHOD(Re2Options, getOneLine);
PHP_METHOD(Re2Options, setEncoding);
PHP_METHOD(Re2Options, setMaxMem);
PHP_METHOD(Re2Options, setPosixSyntax);
PHP_METHOD(Re2Options, setLongestMatch);
PHP_METHOD(Re2Options, setLogErrors);
PHP_METHOD(Re2Options, setLiteral);
PHP_METHOD(Re2Options, setNeverNl);
PHP_METHOD(Re2Options, setCaseSensitive);
PHP_METHOD(Re2Options, setPerlClasses);
PHP_METHOD(Re2Options, setWordBoundary);
PHP_METHOD(Re2Options, setOneLine);

PHP_METHOD(Re2Set, __construct);
PHP_METHOD(Re2Set, add);
PHP_METHOD(Re2Set, compile);
PHP_METHOD(Re2Set, match);

PHP_MINIT_FUNCTION(re2);
PHP_MSHUTDOWN_FUNCTION(re2);
PHP_MINFO_FUNCTION(re2);

#ifdef __cplusplus
}
#endif

#ifdef ZTS
#define RE2_G(v) TSRMG(re2_globals_id, zend_re2_globals *, v)
#else
#define RE2_G(v) (re2_globals.v)
#endif

ZEND_BEGIN_MODULE_GLOBALS(re2)
	zend_bool cache_enabled;
	HashTable *cache_store;
ZEND_END_MODULE_GLOBALS(re2)

#define RE2_OPTIONS_HASH_LEN	8

#endif	/* PHP_RE2_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
