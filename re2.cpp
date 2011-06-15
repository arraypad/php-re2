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
	ZEND_ARG_INFO(1, matches)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_re2_match_all, 0, 0, 3)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, subject)
	ZEND_ARG_INFO(1, matches)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_re2_replace, 0, 0, 3)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, subject)
	ZEND_ARG_INFO(0, replace)
	ZEND_ARG_INFO(0, flags)
	ZEND_ARG_INFO(1, count)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_re2_grep, 0, 0, 2)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, input)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_re2_construct, 0, 0, 1)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_re2_one_arg, 0, 0, 1)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ re2_functions[]
 */
const zend_function_entry re2_functions[] = {
	PHP_FE(re2_match, arginfo_re2_match)
	PHP_FE(re2_match_all, arginfo_re2_match_all)
	PHP_FE(re2_replace, arginfo_re2_replace)
	PHP_FE(re2_grep, arginfo_re2_grep)
	PHP_FE(re2_quote, NULL)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ RE2 classes */
zend_class_entry *php_re2_class_entry;
#define PHP_RE2_CLASS_NAME "RE2"

static zend_function_entry re2_class_functions[] = {
	PHP_ME(RE2, __construct, arginfo_re2_construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(RE2, getOptions, NULL, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

zend_class_entry *php_re2_options_class_entry;
#define PHP_RE2_OPTIONS_CLASS_NAME "RE2_Options"

static zend_function_entry re2_options_class_functions[] = {
	PHP_ME(RE2_Options, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(RE2_Options, getEncoding, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(RE2_Options, getMaxMem, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(RE2_Options, getPosixSyntax, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(RE2_Options, getLongestMatch, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(RE2_Options, getLogErrors, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(RE2_Options, getLiteral, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(RE2_Options, getNeverNl, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(RE2_Options, getCaseSensitive, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(RE2_Options, getPerlClasses, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(RE2_Options, getWordBoundary, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(RE2_Options, getOneLine, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(RE2_Options, setEncoding, arginfo_re2_one_arg, ZEND_ACC_PUBLIC)
	PHP_ME(RE2_Options, setMaxMem, arginfo_re2_one_arg, ZEND_ACC_PUBLIC)
	PHP_ME(RE2_Options, setPosixSyntax, arginfo_re2_one_arg, ZEND_ACC_PUBLIC)
	PHP_ME(RE2_Options, setLongestMatch, arginfo_re2_one_arg, ZEND_ACC_PUBLIC)
	PHP_ME(RE2_Options, setLogErrors, arginfo_re2_one_arg, ZEND_ACC_PUBLIC)
	PHP_ME(RE2_Options, setLiteral, arginfo_re2_one_arg, ZEND_ACC_PUBLIC)
	PHP_ME(RE2_Options, setNeverNl, arginfo_re2_one_arg, ZEND_ACC_PUBLIC)
	PHP_ME(RE2_Options, setCaseSensitive, arginfo_re2_one_arg, ZEND_ACC_PUBLIC)
	PHP_ME(RE2_Options, setPerlClasses, arginfo_re2_one_arg, ZEND_ACC_PUBLIC)
	PHP_ME(RE2_Options, setWordBoundary, arginfo_re2_one_arg, ZEND_ACC_PUBLIC)
	PHP_ME(RE2_Options, setOneLine, arginfo_re2_one_arg, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ RE2 object handlers */
zend_object_handlers re2_object_handlers;
zend_object_handlers re2_options_object_handlers;

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

struct re2_options_object {
	zend_object std;
	RE2::Options *options;
};

void re2_options_free_storage(void *object TSRMLS_DC)
{
	re2_options_object *obj = (re2_options_object *)object;
	delete obj->options;

	zend_hash_destroy(obj->std.properties);
	FREE_HASHTABLE(obj->std.properties);

	efree(obj);
}

zend_object_value re2_options_create_handler(zend_class_entry *type TSRMLS_DC)
{
	zval *tmp;
	zend_object_value retval;

	re2_options_object *obj = (re2_options_object *)emalloc(sizeof(re2_options_object));
	memset(obj, 0, sizeof(re2_options_object));
	obj->std.ce = type;

	ALLOC_HASHTABLE(obj->std.properties);
	zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_copy(obj->std.properties, &type->default_properties, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(obj, NULL, re2_options_free_storage, NULL TSRMLS_CC);
	retval.handlers = &re2_options_object_handlers;
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
	} \
	argc = re2->NumberOfCapturingGroups(); \
	if (argc == -1) { \
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid pattern"); \
		RETURN_FALSE; \
	} \


/* {{{ constants */
#define RE2_ANCHOR_NONE		0
#define RE2_ANCHOR_START	1
#define RE2_ANCHOR_BOTH		2
#define RE2_GREP_INVERT		4
#define RE2_OFFSET_CAPTURE	8
#define RE2_PATTERN_ORDER	16
#define RE2_SET_ORDER		32

#define RE2_REPLACE_GLOBAL	1
#define RE2_REPLACE_FIRST   2
/* }}} */

/*	{{{ match helpers */

static RE2::Anchor _php_re2_get_anchor_from_flags(int flags)
{
	if (flags & RE2_ANCHOR_BOTH) {
		return RE2::ANCHOR_BOTH;
	}

	if (flags & RE2_ANCHOR_START) {
		return RE2::ANCHOR_START;
	}

	return RE2::UNANCHORED;
}

static void _php_re2_populate_matches(RE2 *re2, zval **matches, re2::StringPiece subject_piece, re2::StringPiece *pieces, int argc, long flags)
{
	zval *piece = NULL;
	std::string *str;
	const std::map<int, std::string> named_groups = re2->CapturingGroupNames();

	for (int i = 0, j = 0; i < argc; i++) {
		str = &pieces[i].ToString();

		MAKE_STD_ZVAL(piece);
		if (flags & RE2_OFFSET_CAPTURE) {
			array_init_size(piece, 2);
			add_next_index_stringl(piece, str->c_str(), pieces[i].size(), 1);
			add_next_index_long(piece, pieces[i].data() == NULL ? -1 : pieces[i].data() - subject_piece.data());
		} else {
			ZVAL_STRINGL(piece, str->c_str(), pieces[i].size(), 1);
		}

		std::map<int, std::string>::const_iterator iter = named_groups.find(i);
		if (iter != named_groups.end()) {
			/* this index also has a named group */
			std::string name = iter->second;
			add_assoc_zval_ex(flags & RE2_PATTERN_ORDER ? matches[j++] : *matches, (const char *)name.c_str(), name.length() + 1, piece);
		}

		add_next_index_zval(flags & RE2_PATTERN_ORDER ? matches[j++] : *matches, piece);
	}
}

static long _php_re2_match_common(RE2 *re2, zval **matches, zval *matches_out,
	const char *subject, long subject_len, std::string *out, const char *replace, long replace_len,
	int limit, long offset, int argc, long flags)
{
	const char *start_ptr, *ptr, *end_ptr, *last_ptr = NULL;
	re2::StringPiece subject_piece = re2::StringPiece(subject, subject_len);
	re2::StringPiece replace_piece = re2::StringPiece(replace, replace_len);
	re2::StringPiece pieces[++argc];
	const std::map<int, std::string> named_groups = re2->CapturingGroupNames();
	int num_groups = argc + named_groups.size();
	long end_pos, num_matches = 0;
	bool was_empty = false;
	zval *match_array;

	start_ptr = subject_piece.data();
	ptr = start_ptr + offset;
	end_ptr = subject_piece.end();
	end_pos = subject_piece.size();
	while (ptr < end_ptr && re2->Match(subject_piece, ptr - start_ptr, end_pos, RE2::UNANCHORED, pieces, argc)) {
		if (out && ptr < pieces[0].begin()) {
			out->append(ptr, pieces[0].begin() - ptr);
		}

		if (pieces[0].begin() == last_ptr && pieces[0].size() == 0) {
			/* allow only one empty match at end of last match, to mirror pcre */
			if (was_empty) {
				if (out && ptr < end_ptr) {
					out->append(ptr, 1);
				}

				++ptr;
				continue;
			}
			was_empty = true;
		} else {
			was_empty = false;
		}

		if (flags & RE2_SET_ORDER) {
			/* initialise match arrays per result */
			MAKE_STD_ZVAL(match_array);
			array_init_size(match_array, num_groups);
			matches = &match_array;

			if (matches_out) {
				add_next_index_zval(matches_out, match_array);
			}
		}

		if (out) {
			re2->Rewrite(out, replace_piece, pieces, argc);
		}

		if (matches) {
			_php_re2_populate_matches(re2, matches, subject_piece, pieces, argc, flags);
		}

		ptr = pieces[0].end();
		last_ptr = ptr;
		++num_matches;

		if (limit && num_matches == limit) {
			break;
		}
	}

	if (out && ptr < end_ptr) {
		out->append(ptr, end_ptr - ptr);
	}
	
	return num_matches;
}
/*	}}} */

/*	{{{ proto bool re2_match(mixed $pattern, string $subject [, array &$matches [, int $flags = RE2_ANCHOR_NONE [, int $offset = 0]]])
	Returns whether the pattern matches the subject.
*/
PHP_FUNCTION(re2_match)
{
	char *subject;
	re2::StringPiece subject_piece;
	int subject_len, argc;
	long flags, offset;
	zval *pattern = NULL, *matches = NULL;
	RE2 *re2;
	RE2::Anchor anchor;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zs|zll", &pattern, &subject, &subject_len, &matches, &flags, &offset) == FAILURE) {
		RETURN_FALSE;
	}

	RE2_GET_PATTERN;

	if (ZEND_NUM_ARGS() < 4) {
		/* default flags */
		flags = RE2_ANCHOR_NONE;
	}

	if (ZEND_NUM_ARGS() < 5) {
		offset = 0;
	}

	subject_piece = re2::StringPiece(subject);
	anchor = _php_re2_get_anchor_from_flags(flags);

	if (ZEND_NUM_ARGS() > 2) {
		re2::StringPiece pieces[++argc];

		if (re2->Match(subject_piece, offset, subject_piece.size(), anchor, pieces, argc)) {
			if (matches != NULL) {
				zval_dtor(matches);
			}

			array_init_size(matches, argc);
			_php_re2_populate_matches(re2, &matches, subject_piece, pieces, argc, flags);
			RETURN_TRUE;
		} else {
			RETURN_FALSE;
		}
	} else {
		if (re2->Match(subject_piece, offset, subject_piece.size(), anchor, NULL, 0)) {
			RETURN_TRUE;
		} else {
			RETURN_FALSE;
		}
	}
}
/*	}}} */

/*	{{{ proto int re2_match_all(mixed $pattern, string $subject, array &$matches [, int $flags = 0 [, int $offset = 0]])
	Returns how many times the pattern matched the subject. */
PHP_FUNCTION(re2_match_all)
{
	const char *subject;
	int subject_len, argc, end_pos, num_matches = 0;
	long flags = 0, offset = 0;
	zval *pattern = NULL, *matches_out = NULL, *match_array = NULL, **matches = NULL;
	RE2 *re2;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zsz|ll", &pattern, &subject, &subject_len, &matches_out, &flags, &offset) == FAILURE) {
		RETURN_FALSE;
	}

	RE2_GET_PATTERN;

	if (ZEND_NUM_ARGS() < 4 || !(flags & RE2_SET_ORDER)) {
		/* default to pattern order */
		flags |= RE2_PATTERN_ORDER;
	}

	/* initialise output array */
	if (matches_out != NULL) {
		zval_dtor(matches_out);
	}
	array_init(matches_out);

	if (flags & RE2_PATTERN_ORDER) {
		/* initialise match arrays per group */
		int i, j;
		const std::map<int, std::string> named_groups = re2->CapturingGroupNames();
		int num_groups = 1 + argc + named_groups.size();
		zval **match_arrays = (zval **)emalloc(num_groups * sizeof(zval *));
		matches = match_arrays;

		for (i = 0, j = 0; i < 1 + argc; i++) {
			MAKE_STD_ZVAL(match_array);
			array_init(match_array);
			std::map<int, std::string>::const_iterator iter = named_groups.find(i);
			if (iter != named_groups.end()) {
				/* this index also has a named group */
				std::string name = iter->second;
				add_assoc_zval_ex(matches_out, (const char *)name.c_str(), name.length() + 1, match_array);
				match_arrays[j++] = match_array;
				MAKE_STD_ZVAL(match_array);
				array_init(match_array);
			}
			add_next_index_zval(matches_out, match_array);
			match_arrays[j++] = match_array;
		}
	}

	num_matches = _php_re2_match_common(re2, matches, matches_out, subject, subject_len, NULL, NULL, 0, 0, offset, argc, flags);

	if (flags & RE2_PATTERN_ORDER) {
		efree(matches);
	}

	RETVAL_LONG(num_matches);
}
/*	}}} */

/*	{{{ proto string re2_replace(mixed $pattern, string $replacement, string $subject [, int $flags = RE2_REPLACE_GLOBAL [, int &$count]])
	Replaces all matches of the pattern with the replacement. */
PHP_FUNCTION(re2_replace)
{
	char *subject, *replace;
	std::string pattern_str, out_str = "";
	int subject_len, replace_len, argc, limit = 0;
	long flags = RE2_REPLACE_GLOBAL, num_matches;
	zval *pattern, *count_zv, *out;
	RE2 *re2;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zss|lz", &pattern, &replace, &replace_len, &subject, &subject_len, &flags, &count_zv) == FAILURE) {
		RETURN_FALSE;
	}

	RE2_GET_PATTERN;

	if (ZEND_NUM_ARGS() > 3 && flags & RE2_REPLACE_FIRST) {
		limit = 1;
	}

	num_matches = _php_re2_match_common(re2, NULL, NULL, subject, subject_len, &out_str, replace, replace_len, limit, 0, argc, flags);
	RETVAL_STRINGL(out_str.c_str(), out_str.length(), 1);

	if (ZEND_NUM_ARGS() == 5) {
		ZVAL_LONG(count_zv, num_matches);
	}
}
/*	}}} */

/*	{{{ proto array re2_grep(mixed $pattern, array $subject [, int $flags = RE2_ANCHOR_NONE])
	Return array entries which match the pattern (or which don't, with RE2_GREP_INVERT.) */
PHP_FUNCTION(re2_grep)
{
	int argc;
	long flags;
	zval *pattern, *input, **entry;
	RE2 *re2;
	bool did_match, invert;
	char *string_key;
	ulong num_key;
	RE2::Anchor anchor;
	re2::StringPiece subject_piece;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "za|l", &pattern, &input, &flags) == FAILURE) {
		RETURN_FALSE;
	}

	RE2_GET_PATTERN;

	if (ZEND_NUM_ARGS() < 3) {
		/* default flags */
		flags = RE2_ANCHOR_NONE;
	}

	invert = flags & RE2_GREP_INVERT;
	anchor = _php_re2_get_anchor_from_flags(flags);

	array_init(return_value);
	zend_hash_internal_pointer_reset(Z_ARRVAL_P(input));
	while (zend_hash_get_current_data(Z_ARRVAL_P(input), (void **)&entry) == SUCCESS) {
		zval subject = **entry;

		if (Z_TYPE_PP(entry) != IS_STRING) {
			zval_copy_ctor(&subject);
			convert_to_string(&subject);
		}

		subject_piece = re2::StringPiece(Z_STRVAL(subject));
		did_match = re2->Match(subject_piece, 0, Z_STRLEN(subject), anchor, NULL, 0);

		if (did_match ^ invert) {
			Z_ADDREF_PP(entry);

			switch (zend_hash_get_current_key(Z_ARRVAL_P(input), &string_key, &num_key, 0)) {
				case HASH_KEY_IS_STRING:
					zend_hash_update(Z_ARRVAL_P(return_value), string_key, strlen(string_key) + 1, entry, sizeof(zval *), NULL);
					break;
				case HASH_KEY_IS_LONG:
					zend_hash_index_update(Z_ARRVAL_P(return_value), num_key, entry, sizeof(zval *), NULL);
					break;
			}
		}

		if (Z_TYPE_PP(entry) != IS_STRING) {
			zval_dtor(&subject);
		}

		zend_hash_move_forward(Z_ARRVAL_P(input));
	}
	zend_hash_internal_pointer_reset(Z_ARRVAL_P(input));
}
/*	}}} */

/*	{{{	proto string re2_quote(string $subject)
	Escapes all potentially meaningful regexp characters in the subject. */
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
/*	}}} */

/*	{{{ proto RE2 RE2::__construct(string $pattern [, RE2_Options $options])
	Construct a new RE2 object from the given pattern. */
PHP_METHOD(RE2, __construct)
{
	char *pattern;
	int pattern_len;
	zval *options;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|O", &pattern, &pattern_len, &options, php_re2_options_class_entry) == FAILURE) {
		RETURN_NULL();
	}

	if (ZEND_NUM_ARGS() == 1) {
		zval *ctor, unused;

		/* create options */
		MAKE_STD_ZVAL(options);
		Z_TYPE_P(options) = IS_OBJECT;
		object_init_ex(options, php_re2_options_class_entry);

		MAKE_STD_ZVAL(ctor);
		array_init_size(ctor, 2);
		Z_ADDREF_P(options);
		add_next_index_zval(ctor, options);
		add_next_index_string(ctor, "__construct", 1);
		if (call_user_function(&php_re2_options_class_entry->function_table, &options, ctor, &unused, 0, NULL TSRMLS_CC) == FAILURE) {
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to construct RE2_Options");
			RETURN_NULL();
		}
		zval_ptr_dtor(&ctor);
		Z_DELREF_P(options);
	}

	zend_update_property(php_re2_class_entry, getThis(), "options", strlen("options"), options TSRMLS_CC);

	/* create re2 object */
	re2_options_object *options_obj = (re2_options_object *)zend_object_store_get_object(options TSRMLS_CC);
	RE2 *re2_obj = new RE2(pattern, *options_obj->options);
	re2_object *obj = (re2_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	obj->re2 = re2_obj;
}
/*	}}} */

/* {{{ RE2 options */

/*	{{{ proto RE2_Options RE2::getOptions()
	Returns the RE2_Options for this instance. */
PHP_METHOD(RE2, getOptions)
{
	zval *options = zend_read_property(php_re2_class_entry, getThis(), "options", strlen("options"), 0 TSRMLS_CC);
	Z_ADDREF_P(options);
	RETURN_ZVAL(options, 1, 0);
}
/*	}}} */

/*	{{{ proto RE2_Options RE2::__construct()
	Constructs a new RE2_Options object. */
PHP_METHOD(RE2_Options, __construct)
{
	RE2::Options *options_obj = new RE2::Options();
	re2_options_object *obj = (re2_options_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	obj->options = options_obj;
}
/*	}}} */

/*	{{{ proto string RE2_Options::getEncoding()
	Returns the encoding to use for the pattern and subject strings, "utf8" or "latin1". */
PHP_METHOD(RE2_Options, getEncoding)
{
	re2_options_object *obj = (re2_options_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	RETURN_STRING(obj->options->encoding() == RE2::Options::EncodingUTF8 ? "utf8" : "latin1", 1);
}
/*	}}} */

/*	{{{ proto void RE2_Options::setEncoding(string $encoding)
	Sets the encoding to use for the pattern and subject strings, "utf8" or "latin1". */
PHP_METHOD(RE2_Options, setEncoding)
{
	char *encoding;
	int encoding_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &encoding, &encoding_len) == FAILURE) {
		RETURN_FALSE;
	}

	re2_options_object *obj = (re2_options_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	obj->options->set_encoding(encoding_len == 4 ? RE2::Options::EncodingUTF8 : RE2::Options::EncodingLatin1);
}
/* }}} */

/*	{{{ proto int RE2_Options::getMaxMem()
	Returns the (approximate) maximum amount of memory this RE2 should use, in bytes. */
PHP_METHOD(RE2_Options, getMaxMem)
{
	re2_options_object *obj = (re2_options_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	RETURN_LONG(obj->options->max_mem());
}
/*	}}} */

/*	{{{ proto void RE2_Options::setMaxMem(int $max_mem)
	Set the (approximate) maximum amount of memory this RE2 should use, in bytes. */
PHP_METHOD(RE2_Options, setMaxMem)
{
	long max_mem;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &max_mem) == FAILURE) {
		RETURN_FALSE;
	}

	re2_options_object *obj = (re2_options_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	obj->options->set_max_mem(max_mem);
}
/*	}}} */

#define RE2_OPTION_BOOL_GETTER(name, re2name) \
	PHP_METHOD(RE2_Options, get##name) \
	{ \
		re2_options_object *obj = (re2_options_object *)zend_object_store_get_object(getThis() TSRMLS_CC); \
		RETURN_BOOL(obj->options->re2name()); \
	}

#define RE2_OPTION_BOOL_SETTER(name, re2name) \
	PHP_METHOD(RE2_Options, set##name) \
	{ \
		zend_bool value; \
		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "b", &value) == FAILURE) { \
			RETURN_FALSE; \
		} \
		re2_options_object *obj = (re2_options_object *)zend_object_store_get_object(getThis() TSRMLS_CC); \
		obj->options->set_##re2name(value); \
	}

/*	{{{ proto bool RE2_Options::getPosixSyntax()
	Returns whether patterns are restricted to POSIX egrep syntax. */
RE2_OPTION_BOOL_GETTER(PosixSyntax, posix_syntax);
/*	}}} */

/*	{{{ proto void RE2_Options::setPosixSyntax(bool $value)
	Sets whether patterns are restricted to POSIX egrep syntax. */
RE2_OPTION_BOOL_SETTER(PosixSyntax, posix_syntax);
/*	}}} */

/*	{{{ proto bool RE2_Options::getLongestMatch()
	Returns whether the pattern will search for the longest match instead of the first. */
RE2_OPTION_BOOL_GETTER(LongestMatch, longest_match);
/*	}}} */

/*	{{{ proto void RE2_Options::setLongestMatch(bool $value)
	Sets whether the pattern will search for the longest match instead of the first. */
RE2_OPTION_BOOL_SETTER(LongestMatch, longest_match);
/*	}}} */

/*	{{{ proto bool RE2_Options::getLogErrors()
	Returns whether syntax and execution errors will be written to stderr. */
RE2_OPTION_BOOL_GETTER(LogErrors, log_errors);
/*	}}} */

/*	{{{ proto void RE2_Options::setLogErrors(bool $value)
	Sets whether syntax and execution errors will be written to stderr. */
RE2_OPTION_BOOL_SETTER(LogErrors, log_errors);
/*	}}} */

/*	{{{ proto bool RE2_Options::getLiteral()
	Returns whether the pattern will be interpreted as a literal string instead of a regex. */
RE2_OPTION_BOOL_GETTER(Literal, literal);
/*	}}} */

/*	{{{ proto void RE2_Options::setLiteral(bool $value)
	Sets whether the pattern will be interpreted as a literal string instead of a regex. */
RE2_OPTION_BOOL_SETTER(Literal, literal);
/*	}}} */

/*	{{{ proto bool RE2_Options::getNeverNl()
	Returns whether the newlines will be matched in the pattern. */
RE2_OPTION_BOOL_GETTER(NeverNl, never_nl);
/*	}}} */

/*	{{{ proto void RE2_Options::setNeverNl(bool $value)
	Sets whether the newlines will be matched in the pattern. */
RE2_OPTION_BOOL_SETTER(NeverNl, never_nl);
/*	}}} */

/*	{{{ proto bool RE2_Options::getCaseSensitive()
	Returns whether the pattern will be interpreted as case sensitive. */
RE2_OPTION_BOOL_GETTER(CaseSensitive, case_sensitive);
/*	}}} */

/*	{{{ proto void RE2_Options::setCaseSensitive(bool $value)
	Sets whether the pattern will be interpreted as case sensitive. */
RE2_OPTION_BOOL_SETTER(CaseSensitive, case_sensitive);
/*	}}} */

/*	{{{ proto bool RE2_Options::getPerlClasses()
	Returns whether Perl's "\d \s \w \D \S \W" classes are allowed in posix_syntax mode. */
RE2_OPTION_BOOL_GETTER(PerlClasses, perl_classes);
/*	}}} */

/*	{{{ proto void RE2_Options::setPerlClasses(bool $value)
	Sets whether Perl's \d \s \w \D \S \W classes are allowed in posix_syntax mode. */
RE2_OPTION_BOOL_SETTER(PerlClasses, perl_classes);
/*	}}} */

/*	{{{ proto bool RE2_Options::getWordBoundary()
	Returns whether the \b and \B assertions are allowed in posix_syntax mode. */
RE2_OPTION_BOOL_GETTER(WordBoundary, word_boundary);
/*	}}} */

/*	{{{ proto void RE2_Options::setWordBoundary(bool $value)
	Returns whether the \b and \B assertions are allowed in posix_syntax mode. */
RE2_OPTION_BOOL_SETTER(WordBoundary, word_boundary);
/*	}}} */

/*	{{{ proto bool RE2_Options::getOneLine()
	Returns whether ^ and $ only match the start and end of the subject in posix_syntax mode. */
RE2_OPTION_BOOL_GETTER(OneLine, one_line);
/*	}}} */

/*	{{{ proto void RE2_Options::setOneLine(bool $value)
	Returns whether ^ and $ only match the start and end of the subject in posix_syntax mode. */
RE2_OPTION_BOOL_SETTER(OneLine, one_line);
/*	}}} */

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
	/* register RE2 class */
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, PHP_RE2_CLASS_NAME, re2_class_functions);
	php_re2_class_entry = zend_register_internal_class(&ce TSRMLS_CC);
	zend_declare_property_null(php_re2_class_entry, "options", strlen("options"), ZEND_ACC_PROTECTED TSRMLS_CC);
	php_re2_class_entry->create_object = re2_create_handler;
	memcpy(&re2_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	re2_object_handlers.clone_obj = NULL;

	/* register RE2_Options class */
	INIT_CLASS_ENTRY(ce, PHP_RE2_OPTIONS_CLASS_NAME, re2_options_class_functions);
	php_re2_options_class_entry = zend_register_internal_class(&ce TSRMLS_CC);
	php_re2_options_class_entry->create_object = re2_options_create_handler;
	memcpy(&re2_options_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	re2_options_object_handlers.clone_obj = NULL;

	/* register constants */
	REGISTER_LONG_CONSTANT("RE2_ANCHOR_NONE", RE2_ANCHOR_NONE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RE2_ANCHOR_START", RE2_ANCHOR_START, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RE2_ANCHOR_BOTH", RE2_ANCHOR_BOTH, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RE2_GREP_INVERT", RE2_GREP_INVERT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RE2_OFFSET_CAPTURE", RE2_OFFSET_CAPTURE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RE2_PATTERN_ORDER", RE2_PATTERN_ORDER, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RE2_SET_ORDER", RE2_SET_ORDER, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RE2_REPLACE_GLOBAL", RE2_REPLACE_GLOBAL, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RE2_REPLACE_FIRST", RE2_REPLACE_FIRST, CONST_CS | CONST_PERSISTENT);

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
