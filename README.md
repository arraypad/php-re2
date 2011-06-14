Introduction
============
php-re2 is a PHP extension which provides an interface to Google's [RE2 regular-expression library](http://code.google.com/p/re2/).

> Backtracking engines are typically full of features and convenient syntactic sugar but can be forced into taking exponential amounts of time on even small inputs. RE2 uses automata theory to guarantee that regular expression searches run in time linear in the size of the input. RE2 implements memory limits, so that searches can be constrained to a fixed amount of memory. RE2 is engineered to use a small fixed C++ stack footprint no matter what inputs or regular expressions it must process; thus RE2 is useful in multithreaded environments where thread stacks cannot grow arbitrarily large.
> 
> On large inputs, RE2 is often much faster than backtracking engines; its use of automata theory lets it apply optimizations that the others cannot.
> 
> Unlike most automata-based engines, RE2 implements almost all the common Perl and PCRE features and syntactic sugars. It also finds the leftmost-first match, the same match that Perl would, and can return submatch information. The one significant exception is that RE2 drops support for backreferences¹ and generalized zero-width assertions, because they cannot be implemented efficiently. The syntax page gives full details.

Functions
=========
The interface is intended to follow ext/pcre (`preg_match()` et al) as closely as possible.

The main differences are:

* Pattern delimiters (the "/" in "/foo/") are not required.
* The functions which accept a pattern will take either a string or an RE2 object for the pattern.
* Whenever an array is passed to receive matched subpatterns, the preceding argument (`$argc`) must contain the number of subpatterns in the pattern.

### bool re2_match(mixed $pattern, string $subject [, int $argc [, array &$matches [, int $flags = RE2_MATCH_PARTIAL ]]])

Returns whether the pattern matches the subject.

### int re2_match_all(mixed $pattern, string $subject, int $argc, array &$matches)

Returns how many times the pattern matched the subject.

### string re2_replace(mixed $pattern, string $replacement, string $subject [, int $flags = RE2_REPLACE_GLOBAL [, int &$count]])

Replaces all matches of the pattern with the replacement.

### string re2_quote(string $subject)

Escapes all potentially meaningful regexp characters in the subject.

Classes
=======

## RE2

Represents a compiled regex pattern.

### RE2 RE2::__construct(string $pattern [, RE2_Options $options])

## RE2_Options

Options to be used for a particular pattern.

#### RE2_Options RE2_Options::__construct()

### string RE2_Options::getEncoding()
### void RE2_Options::setEncoding(string $encoding)

Default "utf8".
The encoding to use for the pattern and subject strings, "utf8" or "latin1".

### int RE2_Options::getMaxMem()
### void RE2_Options::setMaxMem(int $max_mem)

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

### bool RE2_Options::getPosixSyntax()
### void RE2_Options::setPosixSyntax(bool $value)

Default `false`.
Restrict patterns to POSIX egrep syntax.

### bool RE2_Options::getLongestMatch()
### void RE2_Options::setLongestMatch(bool $value)

Default `false`.
Search for the longest match instead of the first match.

### bool RE2_Options::getLogErrors()
### void RE2_Options::setLogErrors(bool $value)

Default `true`.
Write syntax and execution errors to stderr.

### bool RE2_Options::getLiteral()
### void RE2_Options::setLiteral(bool $value)

Default `false`.
Interpret pattern as literal, not regex.

### bool RE2_Options::getNeverNl()
### void RE2_Options::setNeverNl(bool $value)

Default `false`.
Never match `\n`, even in regex.

### bool RE2_Options::getCaseSensitive()
### void RE2_Options::setCaseSensitive(bool $value)

Default `true`.
Match is case-sensitive (regexp can override with (?i) unless in posix_syntax mode)

### bool RE2_Options::getPerlClasses()
### void RE2_Options::setPerlClasses(bool $value)

Default `false`.
Allow Perl's `\d \s \w \D \S \W` when in posix_syntax mode.

### bool RE2_Options::getWordBoundary()
### void RE2_Options::setWordBoundary(bool $value)

Default `false`.
Allow `\b \B` (word boundary and not) when in posix_syntax mode.

### bool RE2_Options::getOneLine()
### void RE2_Options::setOneLine(bool $value)

Default `false`.
`^` and `$` only match beginning and end of text when in posix_syntax mode.
