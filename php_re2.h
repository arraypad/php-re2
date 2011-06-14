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

#ifndef PHP_RE2_H
#define PHP_RE2_H

#define PHP_RE2_EXTVER "0.0.1-dev"

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

PHP_MINIT_FUNCTION(re2);
PHP_MSHUTDOWN_FUNCTION(re2);
PHP_RINIT_FUNCTION(re2);
PHP_RSHUTDOWN_FUNCTION(re2);
PHP_MINFO_FUNCTION(re2);

#ifdef __cplusplus
}
#endif

#ifdef ZTS
#define RE2_G(v) TSRMG(re2_globals_id, zend_re2_globals *, v)
#else
#define RE2_G(v) (re2_globals.v)
#endif

#endif	/* PHP_RE2_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */