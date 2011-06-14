--TEST--
re2 - re2_match
--FILE--
<?php

echo "*** Testing re2_match(): no subpatterns, positive (default) partial match\n";
var_dump(re2_match('Hello \w+ world', 'Hello regex world!'));

echo "*** Testing re2_match(): no subpatterns, negative (default) partial match\n";
var_dump(re2_match('Hello \w+ world', 'Hello "regex" world!'));

echo "*** Testing re2_match(): no subpatterns, positive (explicit) partial match\n";
var_dump(re2_match('Hello \w+ world', 'Hello regex world!', 0, $matches, RE2_MATCH_PARTIAL));

echo "*** Testing re2_match(): no subpatterns, negative (explicit) partial match\n";
var_dump(re2_match('Hello \w+ world', 'Hello "regex" world!', 0, $matches, RE2_MATCH_PARTIAL));

echo "*** Testing re2_match(): no subpatterns, positive full match\n";
var_dump(re2_match('Hello \w+ world!', 'Hello regex world!', 0, $matches, RE2_MATCH_FULL));

echo "*** Testing re2_match(): no subpatterns, negative full match\n";
var_dump(re2_match('Hello \w+ world', 'Hello regex world!', 0, $matches, RE2_MATCH_FULL));

echo "*** Testing re2_match(): subpattern used but no argc or matches arguments passed\n";
var_dump(re2_match('Hello (\w+) world', 'Hello regex" world'));

echo "*** Testing re2_match(): argc passed but no matches passed\n";
var_dump(re2_match('Hello (\w+) world', 'Hello regex world', 1));

echo "*** Testing re2_match(): argc=0\n";
var_dump(re2_match('Hello \w+ world', 'Hello regex world', 0, $matches));

echo "*** Testing re2_match(): 1 subpattern\n";
var_dump(re2_match('Hello (\w+) world', 'Hello regex world', 1, $matches), $matches);

echo "*** Testing re2_match(): 3 subpatterns\n";
var_dump(re2_match('(Hello) (\w+) (.*)', 'Hello regex world', 3, $matches), $matches);

?>
--EXPECTF--
*** Testing re2_match(): no subpatterns, positive (default) partial match
bool(true)
*** Testing re2_match(): no subpatterns, negative (default) partial match
bool(false)
*** Testing re2_match(): no subpatterns, positive (explicit) partial match
bool(true)
*** Testing re2_match(): no subpatterns, negative (explicit) partial match
bool(false)
*** Testing re2_match(): no subpatterns, positive full match
bool(true)
*** Testing re2_match(): no subpatterns, negative full match
bool(false)
*** Testing re2_match(): subpattern used but no argc or matches arguments passed
bool(false)
*** Testing re2_match(): argc passed but no matches passed

Warning: re2_match(): Number of subpatterns argument passed but no matches argument in %s on line %d
bool(false)
*** Testing re2_match(): argc=0
bool(true)
*** Testing re2_match(): 1 subpattern
bool(true)
array(1) {
  [0]=>
  string(5) "regex"
}
*** Testing re2_match(): 3 subpatterns
bool(true)
array(3) {
  [0]=>
  string(5) "Hello"
  [1]=>
  string(5) "regex"
  [2]=>
  string(5) "world"
}
