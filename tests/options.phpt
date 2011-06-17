--TEST--
re2 - options
--FILE--
<?php

function testOption($option, $pattern, $subject, $argc = 0, $isPosix = false, $values = array(false, true)) {
  $func = 'set' . $option;

  $opts = new RE2_Options();
  if ($isPosix) $opts->setPosixSyntax(true);
  $opts->$func($values[0]);
  var_dump(re2_match(new RE2($pattern, $opts), $subject, $matches));
  if ($argc) var_dump($matches);

  $opts = new RE2_Options();
  if ($isPosix) $opts->setPosixSyntax(true);
  $opts->$func($values[1]);
  var_dump(re2_match(new RE2($pattern, $opts), $subject, $matches));
  if ($argc) var_dump($matches);
}

echo "*** Testing RE2 option: encoding\n";
testOption('Encoding', "\xC2", "\xC2\xBC", 0, false, array('utf8', 'latin1'));

echo "*** Testing RE2 option: max_mem\n";
testOption('MaxMem', '(a+)*', 'aaaaaaaaa', 0, false, array(2<<3, 2<<20));

echo "*** Testing RE2 option: posix_syntax\n";
testOption('PosixSyntax', '\w+', 'Hello regex world');

echo "*** Testing RE2 option: longest_match\n";
testOption('LongestMatch', 'a?|(a*)', 'aa', 1, 1);

echo "*** Testing RE2 option: log_errors\n";
testOption('LogErrors', '\X', 'Hello regex world');

echo "*** Testing RE2 option: literal\n";
testOption('Literal', '\w', 'Hello regex world');

echo "*** Testing RE2 option: never_nl\n";
testOption('NeverNl', '\n', "Hello regex world\n");

echo "*** Testing RE2 option: case_sensitive\n";
testOption('CaseSensitive', 'hello', 'Hello regex world');

echo "*** Testing RE2 option: perl_classes\n";
testOption('PerlClasses', '\w', 'Hello regex world', 0, true);

echo "*** Testing RE2 option: word_boundary\n";
testOption('WordBoundary', 'Hello\b', 'Hello regex world', 0, true);

echo "*** Testing RE2 option: one_line\n";
testOption('OneLine', '^regex', "Hello\nregex world", 0, true);

?>
--EXPECTF--
*** Testing RE2 option: encoding
re2/re2.cc:%d: Error parsing '%s': invalid UTF-8

Warning: re2_match(): Invalid pattern in %s on line %d
bool(false)
int(1)
*** Testing RE2 option: max_mem
re2/re2.cc:%d: Error compiling '(a+)*'

Warning: re2_match(): Invalid pattern in %s on line %d
bool(false)
int(1)
*** Testing RE2 option: posix_syntax
int(1)
re2/re2.cc:%d: Error parsing '\w+': invalid escape sequence: \w

Warning: re2_match(): Invalid pattern in %s on line %d
bool(false)
*** Testing RE2 option: longest_match
int(1)
array(2) {
  [0]=>
  string(1) "a"
  [1]=>
  string(0) ""
}
int(1)
array(2) {
  [0]=>
  string(2) "aa"
  [1]=>
  string(2) "aa"
}
*** Testing RE2 option: log_errors

Warning: re2_match(): Invalid pattern in %s on line %d
bool(false)
re2/re2.cc:%d: Error parsing '\X': invalid escape sequence: \X

Warning: re2_match(): Invalid pattern in %s on line %d
bool(false)
*** Testing RE2 option: literal
int(1)
int(0)
*** Testing RE2 option: never_nl
int(1)
int(0)
*** Testing RE2 option: case_sensitive
int(1)
int(0)
*** Testing RE2 option: perl_classes
re2/re2.cc:%d: Error parsing '\w': invalid escape sequence: \w

Warning: re2_match(): Invalid pattern in %s on line %d
bool(false)
int(1)
*** Testing RE2 option: word_boundary
re2/re2.cc:%d: Error parsing 'Hello\b': invalid escape sequence: \b

Warning: re2_match(): Invalid pattern in %s on line %d
bool(false)
int(1)
*** Testing RE2 option: one_line
int(1)
int(0)

