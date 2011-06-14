--TEST--
re2 - re2_match
--FILE--
<?php

echo "*** Testing re2_match(): no subpatterns, positive (default) unanchored match\n";
var_dump(re2_match('Hello \w+ world', 'Hello regex world!'));

echo "*** Testing re2_match(): no subpatterns, negative (default) unanchored match\n";
var_dump(re2_match('Hello \w+ world', 'Hello "regex" world!'));

echo "*** Testing re2_match(): no subpatterns, positive (explicit) unanchored match\n";
var_dump(re2_match('Hello \w+ world', 'Hello regex world!', $matches, RE2_ANCHOR_NONE));

echo "*** Testing re2_match(): no subpatterns, negative (explicit) unanchored match\n";
var_dump(re2_match('Hello \w+ world', 'Hello "regex" world!', $matches, RE2_ANCHOR_NONE));

echo "*** Testing re2_match(): no subpatterns, positive ANCHOR_BOTH\n";
var_dump(re2_match('Hello \w+ world!', 'Hello regex world!', $matches, RE2_ANCHOR_BOTH));

echo "*** Testing re2_match(): no subpatterns, negative ANCHOR_BOTH\n";
var_dump(re2_match('Hello \w+ world', 'Hello regex world!', $matches, RE2_ANCHOR_BOTH));

echo "*** Testing re2_match(): no subpatterns, positive ANCHOR_START\n";
var_dump(re2_match('Hello \w+ world', 'Hello regex world!', $matches, RE2_ANCHOR_START));

echo "*** Testing re2_match(): no subpatterns, negative ANCHOR_START\n";
var_dump(re2_match('\w+ world', 'Hello regex world', $matches, RE2_ANCHOR_START));

echo "*** Testing re2_match(): subpattern used but no matches arguments passed\n";
var_dump(re2_match('Hello (\w+) world', 'Hello regex world'));

echo "*** Testing re2_match(): offset\n";
var_dump(re2_match('\w+', 'Hello regex world', $matches, RE2_ANCHOR_NONE, 5), $matches);

echo "*** Testing re2_match(): 1 subpattern\n";
var_dump(re2_match('Hello (\w+) world', 'Hello regex world', $matches), $matches);

echo "*** Testing re2_match(): 3 subpatterns\n";
var_dump(re2_match('(Hello) (\w+) (.*)', 'Hello regex world', $matches), $matches);

echo "*** Testing re2_match(): named groups\n";
var_dump(re2_match('(?P<foo>Hello) (\w+) (?P<bar>.*)', 'Hello regex world', $matches), $matches);

?>
--EXPECTF--
*** Testing re2_match(): no subpatterns, positive (default) unanchored match
bool(true)
*** Testing re2_match(): no subpatterns, negative (default) unanchored match
bool(false)
*** Testing re2_match(): no subpatterns, positive (explicit) unanchored match
bool(true)
*** Testing re2_match(): no subpatterns, negative (explicit) unanchored match
bool(false)
*** Testing re2_match(): no subpatterns, positive ANCHOR_BOTH
bool(true)
*** Testing re2_match(): no subpatterns, negative ANCHOR_BOTH
bool(false)
*** Testing re2_match(): no subpatterns, positive ANCHOR_START
bool(true)
*** Testing re2_match(): no subpatterns, negative ANCHOR_START
bool(false)
*** Testing re2_match(): subpattern used but no matches arguments passed
bool(true)
*** Testing re2_match(): offset
bool(true)
array(1) {
  [0]=>
  string(5) "regex"
}
*** Testing re2_match(): 1 subpattern
bool(true)
array(2) {
  [0]=>
  string(17) "Hello regex world"
  [1]=>
  string(5) "regex"
}
*** Testing re2_match(): 3 subpatterns
bool(true)
array(4) {
  [0]=>
  string(17) "Hello regex world"
  [1]=>
  string(5) "Hello"
  [2]=>
  string(5) "regex"
  [3]=>
  string(5) "world"
}
*** Testing re2_match(): named groups
bool(true)
array(4) {
  [0]=>
  string(17) "Hello regex world"
  ["foo"]=>
  string(5) "Hello"
  [1]=>
  string(5) "regex"
  ["bar"]=>
  string(5) "world"
}
