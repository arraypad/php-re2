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
#include <re2/set.h>
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
ZEND_BEGIN_ARG_INFO_EX(arginfo_re2_split, 0, 0, 2)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, subject)
	ZEND_ARG_INFO(0, limit)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_re2_construct, 0, 0, 1)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_re2_one_arg, 0, 0, 1)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_re2_set_construct, 0, 0, 1)
	ZEND_ARG_INFO(0, options)
	ZEND_ARG_INFO(0, anchor)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_re2_set_add, 0, 0, 2)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, error)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_re2_set_match, 0, 0, 2)
	ZEND_ARG_INFO(0, subject)
	ZEND_ARG_INFO(0, match_indexes)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ re2_functions[]
 */
const zend_function_entry re2_functions[] = {
	PHP_FE(re2_match, arginfo_re2_match)
	PHP_FE(re2_match_all, arginfo_re2_match_all)
	PHP_FE(re2_replace, arginfo_re2_replace)
	PHP_FE(re2_replace_callback, arginfo_re2_replace)
	PHP_FE(re2_filter, arginfo_re2_replace)
	PHP_FE(re2_grep, arginfo_re2_grep)
	PHP_FE(re2_split, arginfo_re2_split)
	PHP_FE(re2_quote, arginfo_re2_one_arg)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ RE2 classes */
zend_class_entry *php_re2_class_entry;
#define PHP_RE2_CLASS_NAME "RE2"

static zend_function_entry re2_class_functions[] = {
	PHP_ME(RE2, __construct, arginfo_re2_construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(RE2, getOptions, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(RE2, getPattern, NULL, ZEND_ACC_PUBLIC)
	PHP_MALIAS(RE2, __toString, getPattern, NULL, ZEND_ACC_PUBLIC)
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

zend_class_entry *php_re2_set_class_entry;
#define PHP_RE2_SET_CLASS_NAME "RE2_Set"

static zend_function_entry re2_set_class_functions[] = {
	PHP_ME(RE2_Set, __construct, arginfo_re2_set_construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(RE2_Set, add, arginfo_re2_set_add, ZEND_ACC_PUBLIC)
	PHP_ME(RE2_Set, compile, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(RE2_Set, match, arginfo_re2_set_match, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

/* }}} */

/* {{{ RE2 object handlers */
zend_object_handlers re2_object_handlers;
zend_object_handlers re2_options_object_handlers;
zend_object_handlers re2_set_object_handlers;

struct re2_object {
	zend_object std;
	RE2 *re;
};

struct re2_options_object {
	zend_object std;
	RE2::Options *options;
};

struct re2_set_object {
	zend_object std;
	RE2::Set *re2_set;
};

void re2_free_storage(void *object TSRMLS_DC)
{
	re2_object *obj = (re2_object *)object;
	delete obj->re;

	zend_hash_destroy(obj->std.properties);
	FREE_HASHTABLE(obj->std.properties);

	efree(obj);
}

zend_object_value re2_object_new_ex(zend_class_entry *type, re2_object **ptr TSRMLS_DC)
{
	zval *tmp;
	zend_object_value retval;
	re2_object *obj;

	obj = (re2_object *)emalloc(sizeof(re2_object));
	memset(obj, 0, sizeof(re2_object));

	if (ptr) {
		*ptr = obj;
	}

	zend_object_std_init(&obj->std, type TSRMLS_CC);
	zend_hash_copy(obj->std.properties, &type->default_properties, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(obj, NULL, re2_free_storage, NULL TSRMLS_CC);
	retval.handlers = &re2_object_handlers;
	return retval;
}

zend_object_value re2_object_new(zend_class_entry *type TSRMLS_DC)
{
	return re2_object_new_ex(type, NULL TSRMLS_CC);
}

zend_object_value re2_object_clone(zval *this_ptr TSRMLS_DC)
{
	re2_options_object *options_obj = NULL;
	zval *options = NULL;
	re2_object *new_obj = NULL;
	re2_object *old_obj = (re2_object *)zend_object_store_get_object(this_ptr TSRMLS_CC);
	zend_object_value retval = re2_object_new_ex(old_obj->std.ce, &new_obj TSRMLS_CC);

	zend_objects_clone_members(&new_obj->std, retval, &old_obj->std, Z_OBJ_HANDLE_P(this_ptr) TSRMLS_CC);

	new_obj->re = new RE2(old_obj->re->pattern());
	/*
	options = zend_read_property(old_obj->std.ce, this_ptr, "options", strlen("options"), 1 TSRMLS_CC);
	options_obj = (re2_options_object *)zend_object_store_get_object(options TSRMLS_CC);
	new_obj->re = new RE2(old_obj->re->pattern(), *options_obj->options);
	Z_DELREF_P(options);
	*/
	
	return retval;
}

void re2_options_free_storage(void *object TSRMLS_DC)
{
	re2_options_object *obj = (re2_options_object *)object;
	delete obj->options;

	zend_hash_destroy(obj->std.properties);
	FREE_HASHTABLE(obj->std.properties);

	efree(obj);
}

zend_object_value re2_options_object_new_ex(zend_class_entry *type, re2_options_object **ptr TSRMLS_DC)
{
	zval *tmp;
	zend_object_value retval;

	re2_options_object *obj = (re2_options_object *)emalloc(sizeof(re2_options_object));
	memset(obj, 0, sizeof(re2_options_object));
	obj->std.ce = type;

	if (ptr) {
		*ptr = obj;
	}

	ALLOC_HASHTABLE(obj->std.properties);
	zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_copy(obj->std.properties, &type->default_properties, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(obj, NULL, re2_options_free_storage, NULL TSRMLS_CC);
	retval.handlers = &re2_options_object_handlers;
	return retval;
}

zend_object_value re2_options_object_new(zend_class_entry *type TSRMLS_DC)
{
	return re2_options_object_new_ex(type, NULL TSRMLS_CC);
}

zend_object_value re2_options_object_clone(zval *this_ptr TSRMLS_DC)
{
	RE2::Options *options = NULL;
	re2_options_object *new_obj = NULL;
	re2_options_object *old_obj = (re2_options_object *)zend_object_store_get_object(this_ptr TSRMLS_CC);
	zend_object_value retval = re2_options_object_new_ex(old_obj->std.ce, &new_obj TSRMLS_CC);

	zend_objects_clone_members(&new_obj->std, retval, &old_obj->std, Z_OBJ_HANDLE_P(this_ptr) TSRMLS_CC);
	new_obj->options = new RE2::Options();

	/* grrr.. why is Copy() private? */
	new_obj->options->set_encoding(old_obj->options->encoding());
	new_obj->options->set_posix_syntax(old_obj->options->posix_syntax());
	new_obj->options->set_longest_match(old_obj->options->longest_match());
	new_obj->options->set_log_errors(old_obj->options->log_errors());
	new_obj->options->set_max_mem(old_obj->options->max_mem());
	new_obj->options->set_literal(old_obj->options->literal());
	new_obj->options->set_never_nl(old_obj->options->never_nl());
	new_obj->options->set_case_sensitive(old_obj->options->case_sensitive());
	new_obj->options->set_perl_classes(old_obj->options->perl_classes());
	new_obj->options->set_word_boundary(old_obj->options->word_boundary());
	new_obj->options->set_one_line(old_obj->options->one_line());
	
	return retval;
}

void re2_set_free_storage(void *object TSRMLS_DC)
{
	re2_set_object *obj = (re2_set_object *)object;
	delete obj->re2_set;

	zend_hash_destroy(obj->std.properties);
	FREE_HASHTABLE(obj->std.properties);

	efree(obj);
}

zend_object_value re2_set_object_new_ex(zend_class_entry *type, re2_set_object **ptr TSRMLS_DC)
{
	zval *tmp;
	zend_object_value retval;

	re2_set_object *obj = (re2_set_object *)emalloc(sizeof(re2_set_object));
	memset(obj, 0, sizeof(re2_set_object));
	obj->std.ce = type;

	if (ptr) {
		*ptr = obj;
	}

	ALLOC_HASHTABLE(obj->std.properties);
	zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_copy(obj->std.properties, &type->default_properties, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(obj, NULL, re2_set_free_storage, NULL TSRMLS_CC);
	retval.handlers = &re2_set_object_handlers;
	return retval;
}

zend_object_value re2_set_object_new(zend_class_entry *type TSRMLS_DC)
{
	return re2_set_object_new_ex(type, NULL TSRMLS_CC);
}

/* }}} */

/* {{{ constants */
#define RE2_ANCHOR_NONE				0
#define RE2_ANCHOR_START			1
#define RE2_ANCHOR_BOTH				2
#define RE2_GREP_INVERT				4
#define RE2_OFFSET_CAPTURE			8
#define RE2_PATTERN_ORDER			16
#define RE2_SET_ORDER				32
#define RE2_SPLIT_DELIM_CAPTURE		64
#define RE2_SPLIT_NO_EMPTY			128
/* }}} */

/*	{{{ match helpers */

/* _php2_re2_get_anchor_from_flags() {{{ */
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
/*	}}} */

#define RE2_MATCH_TO_ZVAL_PIECE(ptr, len, offset) \
{ \
	char *match = ptr ? estrndup(ptr, len) : NULL; \
	piece = NULL; \
	MAKE_STD_ZVAL(piece); \
	if (flags & RE2_OFFSET_CAPTURE) { \
		array_init_size(piece, 2); \
		add_next_index_stringl(piece, match, len, 1); \
		add_next_index_long(piece, ptr == NULL ? -1 : offset); \
	} else { \
		ZVAL_STRINGL(piece, match, len, 1); \
	} \
	if (match) { \
		efree(match); \
	} \
}

/* _php2_re2_populate_matches() {{{ */
static void _php_re2_populate_matches(RE2 *re, zval **matches, re2::StringPiece subject_piece, re2::StringPiece *pieces, int argc, long flags)
{
	zval *piece = NULL;
	std::string *str;
	const std::map<int, std::string> named_groups = re->CapturingGroupNames();

	for (int i = 0, j = 0; i < argc; i++) {
		RE2_MATCH_TO_ZVAL_PIECE(pieces[i].data(), pieces[i].size(), pieces[i].data() - subject_piece.data());

		std::map<int, std::string>::const_iterator iter = named_groups.find(i);
		if (iter != named_groups.end()) {
			/* this index also has a named group */
			std::string name = iter->second;
			Z_ADDREF_P(piece);
			add_assoc_zval_ex(flags & RE2_PATTERN_ORDER ? matches[j++] : *matches, (const char *)name.c_str(), name.length() + 1, piece);
		}

		add_next_index_zval(flags & RE2_PATTERN_ORDER ? matches[j++] : *matches, piece);
		piece = NULL;
	}
}
/*	}}} */

/*	_php2_re2_get_backref() {{{
	Copied completely from ext/pcre/pcre.c, preg_get_backref */
static int _php_re2_get_backref(const char **str, int *backref)
{
	register char in_brace = 0;
	register const char *walk = *str;

	if (walk[1] == 0)
		return 0;

	if (*walk == '$' && walk[1] == '{') {
		in_brace = 1;
		walk++;
	}
	walk++;

	if (*walk >= '0' && *walk <= '9') {
		*backref = *walk - '0';
		walk++;
	} else
		return 0;

	if (*walk && *walk >= '0' && *walk <= '9') {
		*backref = *backref * 10 + *walk - '0';
		walk++;
	}

	if (in_brace) {
		if (*walk == 0 || *walk != '}')
			return 0;
		else
			walk++;
	}

	*str = walk;
	return 1;
}
/*	}}} */

/* _php2_re2_match_common() {{{ */
static long _php_re2_match_common(RE2 *re, zval **matches, zval *matches_out,
	const char *subject, long subject_len, std::string *out, zval *out_array,
	const char *replace, long replace_len, zend_fcall_info *replace_fci, zend_fcall_info_cache *replace_fci_cache,
	int limit, long offset, int argc, long flags TSRMLS_DC)
{
	const char *start_ptr, *ptr, *end_ptr, *last_ptr = NULL;
	re2::StringPiece subject_piece = re2::StringPiece(subject, subject_len);
	re2::StringPiece pieces[++argc];
	const std::map<int, std::string> named_groups = re->CapturingGroupNames();
	int num_groups = argc + named_groups.size();
	long end_pos, num_matches = 0;
	bool was_empty = false;
	zval *match_array, *piece;

	if (limit < 0) {
		limit = 0;
	}

	start_ptr = subject_piece.data();
	ptr = start_ptr + offset;
	end_ptr = subject_piece.end();
	end_pos = subject_piece.size();
	while (ptr < end_ptr && re->Match(subject_piece, ptr - start_ptr, end_pos, RE2::UNANCHORED, pieces, argc)) {
		if (ptr < pieces[0].begin()) {
			if (out) {
				out->append(ptr, pieces[0].begin() - ptr);
			} else if (out_array) {
				RE2_MATCH_TO_ZVAL_PIECE(ptr, pieces[0].begin() - ptr, ptr - start_ptr);
				add_next_index_zval(out_array, piece);
			}
		} else if (out_array && !(flags & RE2_SPLIT_NO_EMPTY)) {
			/* fudge for re2_split with empty pattern */
			if (was_empty && !(flags & RE2_SPLIT_DELIM_CAPTURE) && (ptr != last_ptr || ptr == start_ptr)) {
				RE2_MATCH_TO_ZVAL_PIECE(pieces[0].data(), pieces[0].size(), ptr - start_ptr);
				add_next_index_zval(out_array, piece);
			} else if (pieces[0].size()) {
				RE2_MATCH_TO_ZVAL_PIECE(ptr, 0, ptr - start_ptr);
				add_next_index_zval(out_array, piece);
			}
		}

		if (pieces[0].begin() == last_ptr && pieces[0].size() == 0) {
			/* allow only one empty match at end of last match, to mirror pcre */
			if (was_empty) {
				if (ptr < end_ptr) {
					if (out) {
						out->append(ptr, 1);
					} else if (out_array) {
						RE2_MATCH_TO_ZVAL_PIECE(ptr, 1, ptr - start_ptr);
						add_next_index_zval(out_array, piece);
					}
				}

				++ptr;

				if (out_array && ptr == end_ptr && !(flags & RE2_SPLIT_NO_EMPTY)) {
					/* fudge for re2_split with empty pattern */
					RE2_MATCH_TO_ZVAL_PIECE(ptr, 0, ptr - start_ptr);
					add_next_index_zval(out_array, piece);

					if (flags & RE2_SPLIT_DELIM_CAPTURE) {
						RE2_MATCH_TO_ZVAL_PIECE(ptr, 0, ptr - start_ptr);
						add_next_index_zval(out_array, piece);
					}
				}
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

		if (matches) {
			_php_re2_populate_matches(re, matches, subject_piece, pieces, argc, flags);
		}

		if (out) {
			if (replace_fci) {
				zval *retval_ptr;
				replace_fci->param_count = 1;
				replace_fci->params = &matches;
				replace_fci->retval_ptr_ptr = &retval_ptr;
				if (zend_call_function(replace_fci, replace_fci_cache TSRMLS_CC) == SUCCESS && replace_fci->retval_ptr_ptr && *replace_fci->retval_ptr_ptr) {
					zval *ret = *replace_fci->retval_ptr_ptr;
					convert_to_string(ret);
					out->append(Z_STRVAL_P(ret), Z_STRLEN_P(ret));
				}
				zval_dtor(retval_ptr);
				efree(retval_ptr);
				zval_ptr_dtor(matches);
			} else {
				const char *walk = replace, *end = replace + replace_len;
				char walk_last = 0;
				int n;

				while (walk < end) {
					if (*walk == '\\' || *walk == '$') {
						if (walk_last == '\\') {
							++walk;
							walk_last = 0;
							continue;
						}
						if (_php_re2_get_backref(&walk, &n)) {
							if (n < argc && pieces[n].size() > 0) {
								out->append(pieces[n].data(), pieces[n].size());
							}
							continue;
						}
					}
					out->push_back(*walk);
					walk_last = *walk;
					++walk;
				}
			}
		} else if (out_array && flags & RE2_SPLIT_DELIM_CAPTURE) {
			for (int i = 1; i < argc; i++) {
				RE2_MATCH_TO_ZVAL_PIECE(pieces[i].begin(), pieces[i].size(), ptr - start_ptr);
				add_next_index_zval(out_array, piece);
			}
		}

		ptr = pieces[0].end();
		last_ptr = ptr;
		++num_matches;

		if (limit && num_matches == limit) {
			break;
		}
	}

	if (ptr < end_ptr) {
		if (out) {
			out->append(ptr, end_ptr - ptr);
		} else if (out_array) {
			RE2_MATCH_TO_ZVAL_PIECE(ptr, end_ptr - ptr, ptr - start_ptr);
			add_next_index_zval(out_array, piece);
		}
	}
	
	return num_matches;
}
/*	}}} */

#define RE2_ENSURE_ARRAY(name) \
	if (Z_TYPE_P(name##s) == IS_ARRAY) { \
		name##_array = name##s; \
		name##_count = zend_hash_num_elements(Z_ARRVAL_P(name##_array)); \
		zend_hash_internal_pointer_reset(Z_ARRVAL_P(name##_array)); \
	} else { \
		MAKE_STD_ZVAL(name##_array); \
		array_init_size(name##_array, 1); \
		Z_ADDREF_P(name##s); \
		add_next_index_zval(name##_array, name##s); \
		created_##name##_array = true; \
	}

#define RE2_FREE_ARRAY(name) \
	if (created_##name##_array) { \
		zval_ptr_dtor(&name##_array); \
	}

#define RE2_FREE_PATTERN \
	if (was_new) { \
		delete re; \
	}

static int _php_re2_get_pattern(zval *pattern, RE2 **re, int *argc, bool *was_new)
{
	*was_new = false;
	if (Z_TYPE_P(pattern) == IS_STRING) {
		std::string pattern_str = std::string(Z_STRVAL_P(pattern), Z_STRLEN_P(pattern));
		*re = new RE2(pattern_str);
		*was_new = true;
	} else if (Z_TYPE_P(pattern) == IS_OBJECT && instanceof_function(Z_OBJCE_P(pattern), php_re2_class_entry TSRMLS_CC)) {
		re2_object *obj = (re2_object *)zend_object_store_get_object(pattern TSRMLS_CC);
		*re = obj->re;
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Pattern must be a string or an RE2 object");
		return FAILURE;
	}

	if (!(*re)->ok()) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid pattern");
		return FAILURE;
	}

	*argc = (*re)->NumberOfCapturingGroups();
	if (*argc == -1) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid pattern");
		return FAILURE;
	}

	return SUCCESS;
}

/* _php2_re2_replace_subject() {{{ */
static int _php_re2_replace_subject(zval *patterns, zval *subject, zval *return_value,
	int *count, long limit, long flags, bool is_filter,
	zval *replaces, zend_fcall_info *replace_fci, zend_fcall_info_cache *replace_fci_cache TSRMLS_DC)
{
	int argc, pattern_i = 0, replace_i = 0, pattern_count = 1, replace_count = 1, replace_len;
	RE2 *re;
	std::string out_str;
	zval *pattern_array = NULL, *replace_array = NULL, **pattern_ptr, **replace_ptr = NULL;
	bool created_pattern_array = false, created_replace_array = false, was_new;
	char *replace_str;
	long num_matches;

	RE2_ENSURE_ARRAY(pattern);

	if (!replace_fci) {
		RE2_ENSURE_ARRAY(replace);
	}

	Z_ADDREF_P(subject);
	SEPARATE_ZVAL(&subject);

	while (zend_hash_get_current_data(Z_ARRVAL_P(pattern_array), (void **)&pattern_ptr) == SUCCESS) {
		if (_php_re2_get_pattern(*pattern_ptr, &re, &argc, &was_new) == FAILURE) {
			RE2_FREE_PATTERN;
			RE2_FREE_ARRAY(pattern);
			RE2_FREE_ARRAY(replace);
			zval_ptr_dtor(&subject);
			ZVAL_FALSE(return_value);
			return FAILURE;
		}

		if (replace_fci || zend_hash_get_current_data(Z_ARRVAL_P(replace_array), (void **)&replace_ptr) == FAILURE) {
			replace_str = "";
			replace_len = 0;
		} else {
			replace_str = Z_STRVAL_PP(replace_ptr);
			replace_len = Z_STRLEN_PP(replace_ptr);
		}

		out_str = "";
		num_matches = _php_re2_match_common(re, NULL, NULL, Z_STRVAL_P(subject), Z_STRLEN_P(subject),
			&out_str, NULL, replace_str, replace_len, replace_fci, replace_fci_cache, limit, 0, argc, flags TSRMLS_CC);

		if (num_matches) {
			*count += num_matches;
			efree(Z_STRVAL_P(subject));
			ZVAL_STRINGL(subject, out_str.c_str(), out_str.length(), 1);
		}

		RE2_FREE_PATTERN;
		if (++pattern_i <= pattern_count) {
			zend_hash_move_forward(Z_ARRVAL_P(pattern_array));
		}

		if (replace_array && !created_replace_array && ++replace_i <= replace_count) {
			zend_hash_move_forward(Z_ARRVAL_P(replace_array));
		}
	}

	RE2_FREE_ARRAY(pattern);
	RE2_FREE_ARRAY(replace);

	Z_TYPE_P(return_value) = IS_STRING;
	ZVAL_STRINGL(return_value, Z_STRVAL_P(subject), Z_STRLEN_P(subject), 1);
	zval_ptr_dtor(&subject);

	return SUCCESS;
}
/*	}}} */

/* _php2_re2_replace_subjects() {{{ */
static void _php_re2_replace_subjects(zval *patterns, zval *subjects, zval *return_value,
	zval *count_zv, long limit, long flags, bool is_filter,
	zval *replaces, zend_fcall_info *replace_fci, zend_fcall_info_cache *replace_fci_cache TSRMLS_DC)
{
	zval **subject_ptr, *subject_return = NULL;
	int count = 0, total_count = 0;

	if (Z_TYPE_P(subjects) == IS_ARRAY) {
		array_init(return_value);
		zend_hash_internal_pointer_reset(Z_ARRVAL_P(subjects));
		while (zend_hash_get_current_data(Z_ARRVAL_P(subjects), (void **)&subject_ptr) == SUCCESS) {
			MAKE_STD_ZVAL(subject_return);
			count = 0;
			if (_php_re2_replace_subject(patterns, *subject_ptr, subject_return, &count, limit, flags, is_filter, replaces, replace_fci, replace_fci_cache TSRMLS_CC) == FAILURE) {
				zval_ptr_dtor(&subject_return);
				zval_ptr_dtor(&return_value);
				MAKE_STD_ZVAL(return_value);
				ZVAL_FALSE(return_value);
				RETVAL_FALSE;
				return;
			}

			if (!is_filter || count) {
				char *string_key = NULL;
				ulong num_key;

				total_count += count;
				Z_ADDREF_P(subject_return);
				switch (zend_hash_get_current_key(Z_ARRVAL_P(subjects), &string_key, &num_key, 0)) {
					case HASH_KEY_IS_STRING:
						add_assoc_zval_ex(return_value, (const char *)string_key, strlen(string_key) + 1, subject_return);
						break;
					case HASH_KEY_IS_LONG:
						zend_hash_index_update(Z_ARRVAL_P(return_value), num_key, &subject_return, sizeof(zval *), NULL);
						break;
				}
			}
			zval_ptr_dtor(&subject_return);
	
			subject_return = NULL;
			zend_hash_move_forward(Z_ARRVAL_P(subjects));
		}
	} else {
		if (_php_re2_replace_subject(patterns, subjects, return_value, &total_count, limit, flags, is_filter, replaces, replace_fci, replace_fci_cache TSRMLS_CC) == FAILURE) {
			RETVAL_FALSE;
			return;
		}

		if (is_filter && !total_count) {
			efree(Z_STRVAL_P(return_value));
			RETVAL_NULL();
		}
	}

	if (count_zv) {
		ZVAL_LONG(count_zv, total_count);
	}
}
/*	}}} */

/*	}}} */

/*	{{{ options object helpers */
/* static inline int _create_re2_options_object(zval *options) {{{ */
static inline int _create_re2_options_object(zval *options)
{
	zval *ctor, unused;

	MAKE_STD_ZVAL(ctor);
	array_init_size(ctor, 2);
	Z_ADDREF_P(options);
	add_next_index_zval(ctor, options);
	add_next_index_string(ctor, "__construct", 1);
	if (call_user_function(&php_re2_options_class_entry->function_table, &options, ctor, &unused, 0, NULL TSRMLS_CC) == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to construct RE2_Options");
		return FAILURE;
	}
	zval_ptr_dtor(&ctor);
	Z_DELREF_P(options);
	return SUCCESS;
}
/* }}} */
#define PHP_RE2_CREATE_OPTIONS_OBJECT \
	MAKE_STD_ZVAL(options); \
	Z_TYPE_P(options) = IS_OBJECT; \
	object_init_ex(options, php_re2_options_class_entry); \
	if(_create_re2_options_object(options) == FAILURE) { \
		RETURN_NULL(); \
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
	long flags = RE2_ANCHOR_NONE, offset = 0;
	zval *pattern = NULL, *matches = NULL;
	RE2 *re;
	RE2::Anchor anchor;
	bool was_new;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zs|zll", &pattern, &subject, &subject_len, &matches, &flags, &offset) == FAILURE) {
		RETURN_FALSE;
	}

	RETVAL_FALSE;

	if (_php_re2_get_pattern(pattern, &re, &argc, &was_new) == FAILURE) {
		RE2_FREE_PATTERN;
		return;
	}

	subject_piece = re2::StringPiece(subject, subject_len);
	anchor = _php_re2_get_anchor_from_flags(flags);

	if (ZEND_NUM_ARGS() > 2) {
		re2::StringPiece pieces[++argc];

		if (re->Match(subject_piece, offset, subject_piece.size(), anchor, pieces, argc)) {
			zval_dtor(matches);
			array_init_size(matches, argc);
			flags |= RE2_SET_ORDER;
			flags &= ~RE2_PATTERN_ORDER;
			_php_re2_populate_matches(re, &matches, subject_piece, pieces, argc, flags);
			RETVAL_LONG(1);
		} else {
			RETVAL_LONG(0);
		}
	} else {
		RETVAL_LONG(re->Match(subject_piece, offset, subject_piece.size(), anchor, NULL, 0));
	}
	
	RE2_FREE_PATTERN;
}
/*	}}} */

/*	{{{ proto int re2_match_all(mixed $pattern, string $subject, array &$matches [, int $flags = RE2_PATTERN_ORDER [, int $offset = 0]])
	Returns how many times the pattern matched the subject. */
PHP_FUNCTION(re2_match_all)
{
	const char *subject;
	int subject_len, argc, num_matches = 0;
	long flags = 0, offset = 0;
	zval *pattern = NULL, *matches_out = NULL, *match_array = NULL, **matches = NULL;
	RE2 *re;
	bool was_new;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zsz|ll", &pattern, &subject, &subject_len, &matches_out, &flags, &offset) == FAILURE) {
		RETURN_FALSE;
	}

	RETVAL_FALSE;
	if (_php_re2_get_pattern(pattern, &re, &argc, &was_new) == FAILURE) {
		RE2_FREE_PATTERN;
		return;
	}

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
		const std::map<int, std::string> named_groups = re->CapturingGroupNames();
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

		flags &= ~RE2_SET_ORDER;
	}

	num_matches = _php_re2_match_common(re, matches, matches_out, subject, subject_len, NULL, NULL, NULL, 0, NULL, NULL, 0, offset, argc, flags TSRMLS_CC);
	RETVAL_LONG(num_matches);

	if (flags & RE2_PATTERN_ORDER) {
		efree(matches);
	}
	
	RE2_FREE_PATTERN;
}
/*	}}} */

/*	{{{ proto string re2_replace(mixed $pattern, mixed $replacement, mixed $subject [, int $limit = -1 [, int &$count]])
	Replaces all matches of the pattern with the replacement. */
PHP_FUNCTION(re2_replace)
{
	long limit = 0, flags = 0;
	zval *patterns, *replaces, *subjects, *count_zv = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz|lz", &patterns, &replaces, &subjects, &limit, &count_zv) == FAILURE) {
		RETURN_FALSE;
	}

	_php_re2_replace_subjects(patterns, subjects, return_value, count_zv, limit, flags, 0, replaces, NULL, NULL TSRMLS_CC);
}
/*	}}} */

/*	{{{ proto string re2_filter(mixed $pattern, mixed $replacement, mixed $subject [, int $limit = -1 [, int &$count]])
	Replaces all matches of the pattern with the replacement. */
PHP_FUNCTION(re2_filter)
{
	long limit = 0, flags = 0;
	zval *patterns, *replaces, *subjects, *count_zv = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz|lz", &patterns, &replaces, &subjects, &limit, &count_zv) == FAILURE) {
		RETURN_FALSE;
	}

	_php_re2_replace_subjects(patterns, subjects, return_value, count_zv, limit, flags, 1, replaces, NULL, NULL TSRMLS_CC);
}
/*	}}} */

/*	{{{ proto string re2_replace_callback(mixed $pattern, mixed $callback, mixed $subject [, int $limit = -1 [, int &$count]])
	Replaces all matches of the pattern with the value returned by the replacement callback. */
PHP_FUNCTION(re2_replace_callback)
{
	zval *patterns, *subjects, *count_zv = NULL;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;
	long limit = 0, flags = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zfz|lz", &patterns, &fci, &fci_cache, &subjects, &limit, &count_zv) == FAILURE) {
		RETURN_FALSE;
	}

	flags |= RE2_SET_ORDER;
	_php_re2_replace_subjects(patterns, subjects, return_value, count_zv, limit, flags, 0, NULL, &fci, &fci_cache TSRMLS_CC);
}
/*	}}} */

/*	{{{ proto array re2_grep(mixed $pattern, array $subject [, int $flags = RE2_ANCHOR_NONE])
	Return array entries which match the pattern (or which don't, with RE2_GREP_INVERT.) */
PHP_FUNCTION(re2_grep)
{
	int argc;
	long flags;
	zval *pattern, *input, **entry;
	RE2 *re;
	bool was_new, did_match, invert;
	char *string_key;
	ulong num_key;
	RE2::Anchor anchor;
	re2::StringPiece subject_piece;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "za|l", &pattern, &input, &flags) == FAILURE) {
		RETURN_FALSE;
	}

	if (_php_re2_get_pattern(pattern, &re, &argc, &was_new) == FAILURE) {
		RE2_FREE_PATTERN;
		RETURN_FALSE;
	}

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

		subject_piece = re2::StringPiece(Z_STRVAL(subject), Z_STRLEN(subject));
		did_match = re->Match(subject_piece, 0, Z_STRLEN(subject), anchor, NULL, 0);

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
	
	RE2_FREE_PATTERN;
}
/*	}}} */

/*	{{{ proto string re2_split(mixed $pattern, string $subject [, int $limit = -1 [, int $flags]])
	Split a string by a regular expression. */
PHP_FUNCTION(re2_split)
{
	char *subject;
	int subject_len, argc;
	long num_matches, limit = 0, flags = 0;
	zval *pattern, *count_zv;
	RE2 *re;
	bool was_new;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zs|ll", &pattern, &subject, &subject_len, &limit, &flags) == FAILURE) {
		RETURN_FALSE;
	}

	if (_php_re2_get_pattern(pattern, &re, &argc, &was_new) == FAILURE) {
		RE2_FREE_PATTERN;
		RETURN_FALSE;
	}

	array_init(return_value);
	--limit; /* limit includes "rest of subject" */
	num_matches = _php_re2_match_common(re, NULL, NULL, subject, subject_len, NULL, return_value, NULL, 0, NULL, NULL, limit, 0, argc, flags TSRMLS_CC);

	RE2_FREE_PATTERN;
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

	subject_str = std::string(subject, subject_len);
	out_str = RE2::QuoteMeta(subject_str);
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
	std::string pattern_str;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|O", &pattern, &pattern_len, &options, php_re2_options_class_entry) == FAILURE) {
		RETURN_NULL();
	}

	if (ZEND_NUM_ARGS() == 1) {
		PHP_RE2_CREATE_OPTIONS_OBJECT
	}

	zend_update_property(php_re2_class_entry, getThis(), "options", strlen("options"), options TSRMLS_CC);

	/* create re2 object */
	pattern_str = std::string(pattern, pattern_len);
	re2_options_object *options_obj = (re2_options_object *)zend_object_store_get_object(options TSRMLS_CC);
	RE2 *re2_obj = new RE2(pattern_str, *options_obj->options);
	re2_object *obj = (re2_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	obj->re = re2_obj;
}
/*	}}} */

/* {{{ RE2 options */

/*	{{{ proto string RE2::getPattern()
	Returns the pattern used by this instance. */
PHP_METHOD(RE2, getPattern)
{
	std::string pattern;
	re2_object *obj = (re2_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	pattern = obj->re->pattern();
	RETURN_STRINGL(pattern.c_str(), pattern.length(), 1);
}
/*	}}} */

/*	{{{ proto RE2_Options RE2::getOptions()
	Returns the RE2_Options for this instance. */
PHP_METHOD(RE2, getOptions)
{
	zval *options = zend_read_property(php_re2_class_entry, getThis(), "options", strlen("options"), 0 TSRMLS_CC);
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

/* {{{ RE2_Set */

/*	{{{ proto RE2_Set RE2_Set::__construct([RE2_Options $options [, int $flags = RE2_ANCHOR_NONE]])
	Construct a new RE2_Set object with options and anchor. */
PHP_METHOD(RE2_Set, __construct)
{
	zval *options;
	long flags = 0;
	RE2::Anchor anchor;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|Ol", &options, php_re2_options_class_entry, &flags) == FAILURE) {
		RETURN_NULL();
	}
		
	if (ZEND_NUM_ARGS() == 0) {
		PHP_RE2_CREATE_OPTIONS_OBJECT
	}

	anchor = _php_re2_get_anchor_from_flags(flags);

	re2_options_object *options_obj = (re2_options_object *)zend_object_store_get_object(options TSRMLS_CC);
	RE2::Set *s = new RE2::Set(*options_obj->options, anchor);
	re2_set_object *obj = (re2_set_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	obj->re2_set = s;
}
/*	}}} */

/*	{{{ proto int RE2_Set::Add(string $pattern)
	 */
PHP_METHOD(RE2_Set, add)
{
	char *pattern;
	int pattern_len;
	long ret;
	std::string error_str;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &pattern, &pattern_len) == FAILURE) {
		RETURN_FALSE;
	}

	re2_set_object *obj = (re2_set_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	ret = obj->re2_set->Add(pattern, &error_str);

	if (ret == -1) {
		const char *error = error_str.c_str();
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid pattern: '%s'", error);
	} else {
		zend_update_property_bool(php_re2_set_class_entry, getThis(), "hasPattern", strlen("hasPattern"), 1 TSRMLS_CC);
	}

	RETURN_LONG(ret);
}
/*	}}} */

/*	{{{ proto bool RE2_Set::Compile()
	 */
PHP_METHOD(RE2_Set, compile)
{
	long ret;
	zval *hasPattern = zend_read_property(php_re2_set_class_entry, getThis(), "hasPattern", strlen("hasPattern"), 0 TSRMLS_CC);

	if (!Z_BVAL_P(hasPattern)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Set has no patterns");
		Z_DELREF_P(hasPattern);
		RETURN_FALSE;
	}

	Z_DELREF_P(hasPattern);
	
	re2_set_object *obj = (re2_set_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	ret = obj->re2_set->Compile();

	if (ret >= 0) {
		zend_update_property_bool(php_re2_set_class_entry, getThis(), "isCompiled", strlen("isCompiled"), 1 TSRMLS_CC);
	} else {
		/* todo: handle compile failure */
	}

	RETURN_BOOL(ret);
}
/*	}}} */

/*	{{{ proto bool RE2_Set::Match(string $subject, array &$matching_indexs)
	 */
PHP_METHOD(RE2_Set, match)
{
	char *subject;
	int subject_len;
	zval *matching_indexs_out = NULL;
	zval *isCompiled = zend_read_property(php_re2_set_class_entry, getThis(), "isCompiled", strlen("isCompiled"), 0 TSRMLS_CC);

	if (!Z_BVAL_P(isCompiled)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Set is not compiled");
		Z_DELREF_P(isCompiled);
		RETURN_FALSE;
	}

	Z_DELREF_P(isCompiled);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &subject, &subject_len, &matching_indexs_out) == FAILURE) {
		RETURN_FALSE;
	}

	/* initialise output array */
	if (matching_indexs_out != NULL) {
		zval_dtor(matching_indexs_out);
	}
	array_init(matching_indexs_out);
	std::vector<int> v;
		
	re2_set_object *obj = (re2_set_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	bool found = obj->re2_set->Match(subject, &v);
		
	if(v.size()) {
		for( std::vector<int>::iterator it=v.begin(); it!=v.end(); ++it ) {
			add_next_index_long(matching_indexs_out, *it);
		}
	}
	
	RETURN_BOOL(found);
}
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
	NULL,
	NULL,
	NULL,
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
	php_re2_class_entry->create_object = re2_object_new;
	memcpy(&re2_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	re2_object_handlers.clone_obj = re2_object_clone;

	/* register RE2_Options class */
	INIT_CLASS_ENTRY(ce, PHP_RE2_OPTIONS_CLASS_NAME, re2_options_class_functions);
	php_re2_options_class_entry = zend_register_internal_class(&ce TSRMLS_CC);
	php_re2_options_class_entry->create_object = re2_options_object_new;
	memcpy(&re2_options_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	re2_options_object_handlers.clone_obj = re2_options_object_clone;

	/* register RE2_Set class */
	INIT_CLASS_ENTRY(ce, PHP_RE2_SET_CLASS_NAME, re2_set_class_functions);
	php_re2_set_class_entry = zend_register_internal_class(&ce TSRMLS_CC);
	zend_declare_property_bool(php_re2_set_class_entry, "hasPattern", strlen("hasPattern"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_bool(php_re2_set_class_entry, "isCompiled", strlen("isCompiled"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);
	php_re2_set_class_entry->create_object = re2_set_object_new;
	memcpy(&re2_set_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	/* todo: re2_set_object_handlers.clone_obj = re2_set_object_clone; */
		
	/* register constants */
	REGISTER_LONG_CONSTANT("RE2_ANCHOR_NONE", RE2_ANCHOR_NONE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RE2_ANCHOR_START", RE2_ANCHOR_START, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RE2_ANCHOR_BOTH", RE2_ANCHOR_BOTH, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RE2_GREP_INVERT", RE2_GREP_INVERT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RE2_OFFSET_CAPTURE", RE2_OFFSET_CAPTURE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RE2_PATTERN_ORDER", RE2_PATTERN_ORDER, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RE2_SET_ORDER", RE2_SET_ORDER, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RE2_SPLIT_DELIM_CAPTURE", RE2_SPLIT_DELIM_CAPTURE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RE2_SPLIT_OFFSET_CAPTURE", RE2_OFFSET_CAPTURE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RE2_SPLIT_NO_EMPTY", RE2_SPLIT_NO_EMPTY, CONST_CS | CONST_PERSISTENT);

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
