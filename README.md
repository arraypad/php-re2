Introduction
============
re2 is a PHP extension which provides an interface to Google's [RE2 regular-expression library](http://code.google.com/p/re2/).

> Backtracking engines are typically full of features and convenient syntactic sugar but can be forced into taking exponential amounts of time on even small inputs. RE2 uses automata theory to guarantee that regular expression searches run in time linear in the size of the input. RE2 implements memory limits, so that searches can be constrained to a fixed amount of memory. RE2 is engineered to use a small fixed C++ stack footprint no matter what inputs or regular expressions it must process; thus RE2 is useful in multithreaded environments where thread stacks cannot grow arbitrarily large.
> 
> On large inputs, RE2 is often much faster than backtracking engines; its use of automata theory lets it apply optimizations that the others cannot.
> 
> Unlike most automata-based engines, RE2 implements almost all the common Perl and PCRE features and syntactic sugars. It also finds the leftmost-first match, the same match that Perl would, and can return submatch information. The one significant exception is that RE2 drops support for backreferences and generalized zero-width assertions, because they cannot be implemented efficiently. The [syntax page](http://code.google.com/p/re2/wiki/Syntax) gives full details.

Usage examples
==============

````php
<?php

$subject = 'Hello regex world';

re2_match_all('\w+', $subject, $matches);
print_r($matches);
/*
Array
(
    [0] => Array
        (
            [0] => Hello
            [1] => regex
            [2] => world
        )

)
*/

re2_match_all('\w(\w+)', $subject, $matches, RE2_SET_ORDER);
print_r($matches);
/*
Array
(
    [0] => Array
        (
            [0] => Hello
            [1] => ello
        )

    [1] => Array
        (
            [0] => regex
            [1] => egex
        )

    [2] => Array
        (
            [0] => world
            [1] => orld
        )

)
*/

echo re2_replace('\w+', 'foo', $subject), "\n";
/*
foo foo foo
*/

echo re2_replace('\w+', 'foo', $subject, 1), "\n";
/*
foo regex world
*/

echo re2_replace_callback('\w+', function($m) { return strtoupper($m[0]); }, $subject, 2), "\n";
/*
HELLO REGEX world
*/

?>
````

Functions
=========
The interface is intended to follow ext/pcre (`preg_match()` et al) as closely as possible.

The main differences are:

* Pattern delimiters (the "/" in "/foo/") are not required.
* The functions which accept a pattern will take either a string or an RE2 object for the pattern.

#### int re2_match(mixed $pattern, string $subject [, array &$matches [, int $flags = RE2_ANCHOR_NONE [, int $offset = 0]]])

Returns whether the pattern matches the subject.

#### int re2_match_all(mixed $pattern, string $subject, array &$matches [, int $flags = RE2_PATTERN_ORDER [, int $offset = 0]])

Returns how many times the pattern matched the subject.

#### mixed re2_replace(mixed $pattern, mixed $replacement, mixed $subject [, int $limit = -1 [, int &$count]])

Replaces all matches of the pattern with the replacement.

#### mixed re2_replace_callback(mixed $pattern, mixed $callback, mixed $subject [, int $limit = -1 [, int &$count]])

Replaces all matches of the pattern with the value returned by the replacement callback.

#### mixed re2_filter(mixed $pattern, mixed $replacement, mixed $subject [, int $limit = -1 [, int &$count]])

Replaces all matches of the pattern with the replacement. Returns only the subjects where there was a match.

#### array re2_grep(mixed $pattern, array $subject [, int $flags = RE2_ANCHOR_NONE])

Return array entries which match the pattern (or which don't, with RE2_GREP_INVERT.)

#### string re2_quote(string $subject)

Escapes all potentially meaningful regexp characters in the subject.

Classes
=======

## Re2

Represents a compiled regex pattern.

#### Re2 Re2::__construct(string $pattern [, RE2_Options $options [, bool $force_cache = false]])

Construct a new Re2 object.
If `$force_cache` is `true` the cache will be used regardless of the re2.cache_enabled ini setting.

#### string Re2::getPattern()

Returns the pattern.

#### Re2Options Re2::getOptions()

Returns the options used for this pattern.

## Re2Options

Options to be used for a particular pattern.

#### Re2Options Re2Options::__construct()

Construct a new Re2Options object.

#### string Re2Options::getEncoding()
#### void Re2Options::setEncoding(string $encoding)

Default "utf8".
The encoding to use for the pattern and subject strings, "utf8" or "latin1".

#### int Re2Options::getMaxMem()
#### void Re2Options::setMaxMem(int $max_mem)

Default 8388608 (65KB).

> The max_mem option controls how much memory can be used
> to hold the compiled form of the regexp (the Prog) and
> its cached DFA graphs.  Code Search placed limits on the number
> of Prog instructions and DFA states: 10,000 for both.
> In RE2, those limits would translate to about 240 KB per Prog
> and perhaps 2.5 MB per DFA (DFA state sizes vary by regexp; RE2 does a
> better job of keeping them small than Code Search did).
> Each RE2 has two Progs (one forward, one reverse), and each Prog
> can have two DFAs (one first match, one longest match).
>
> The RE2 memory budget is statically divided between the two
> Progs and then the DFAs: two thirds to the forward Prog
> and one third to the reverse Prog.  The forward Prog gives half
> of what it has left over to each of its DFAs.  The reverse Prog
> gives it all to its longest-match DFA.
>
> Once a DFA fills its budget, it flushes its cache and starts over.
> If this happens too often, RE2 falls back on the NFA implementation.

#### bool Re2Options::getPosixSyntax()
#### void Re2Options::setPosixSyntax(bool $value)

Default `false`.
Restrict patterns to POSIX egrep syntax.

#### bool Re2Options::getLongestMatch()
#### void Re2Options::setLongestMatch(bool $value)

Default `false`.
Search for the longest match instead of the first match.

#### bool Re2Options::getLogErrors()
#### void Re2Options::setLogErrors(bool $value)

Default `true`.
Write syntax and execution errors to stderr.

#### bool Re2Options::getLiteral()
#### void Re2Options::setLiteral(bool $value)

Default `false`.
Interpret pattern as literal, not regex.

#### bool Re2Options::getNeverNl()
#### void Re2Options::setNeverNl(bool $value)

Default `false`.
Never match `\n`, even in regex.

#### bool Re2Options::getCaseSensitive()
#### void Re2Options::setCaseSensitive(bool $value)

Default `true`.
Match is case-sensitive (regexp can override with (?i) unless in posix_syntax mode)

#### bool Re2Options::getPerlClasses()
#### void Re2Options::setPerlClasses(bool $value)

Default `false`.
Allow Perl's `\d \s \w \D \S \W` when in posix_syntax mode.

#### bool Re2Options::getWordBoundary()
#### void Re2Options::setWordBoundary(bool $value)

Default `false`.
Allow `\b \B` (word boundary and not) when in posix_syntax mode.

#### bool Re2Options::getOneLine()
#### void Re2Options::setOneLine(bool $value)

Default `false`.
`^` and `$` only match beginning and end of text when in posix_syntax mode.

Runtime configuration
=======

#### re2.cache_enabled = false (PHP_INI_ALL)

When set to `true`, uses a cache (per process) to store all compiled patterns. The cache can be used even when `re2.cache_enabled` is set to `false` by passing the `$force_cache` parameter to the Re2 constructor.
