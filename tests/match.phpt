--TEST--
re2 - re2_match
--SKIPIF--
<?php include('skipif.inc'); ?>
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

echo "*** Testing re2_match(): 1 subpattern & offset capture\n";
var_dump(re2_match('Hello (\w+) world', 'Hello regex world', $matches, RE2_OFFSET_CAPTURE), $matches);

echo "*** Testing re2_match(): offset & offset capture\n";
var_dump(re2_match('Hello (\w+) world', 'Hello regex world', $matches, RE2_OFFSET_CAPTURE, 5), $matches);

echo "*** Testing re2_match(): named groups & offset capture\n";
var_dump(re2_match('(?P<foo>Hello) (\w+) (?P<bar>.*)', 'Hello regex world', $matches, RE2_OFFSET_CAPTURE), $matches);

echo "*** Testing re2_match(): absent subpattern\n";
var_dump(re2_match('(foo)|(bar)baz', 'barbazbla', $matches), $matches);

echo "*** Testing re2_match(): absent subpattern, zero width subpattern & offset capture\n";
var_dump(re2_match('(foo)|(bar)(z*)baz', 'barbazbla', $matches, RE2_OFFSET_CAPTURE), $matches);

echo "*** Testing re2_match(): bad flags\n";
var_dump(re2_match('Hello (\w+) world', 'Hello regex world!', $matches, RE2_PATTERN_ORDER | RE2_SPLIT_NO_EMPTY | RE2_GREP_INVERT), $matches);

echo "*** Testing re2_match(): >10 subpatterns\n";
var_dump(re2_match('Hello (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)', 'Hello abcdefghijklm', $matches), $matches);

?>
--EXPECTF--
*** Testing re2_match(): no subpatterns, positive (default) unanchored match
int(1)
*** Testing re2_match(): no subpatterns, negative (default) unanchored match
int(0)
*** Testing re2_match(): no subpatterns, positive (explicit) unanchored match
int(1)
*** Testing re2_match(): no subpatterns, negative (explicit) unanchored match
int(0)
*** Testing re2_match(): no subpatterns, positive ANCHOR_BOTH
int(1)
*** Testing re2_match(): no subpatterns, negative ANCHOR_BOTH
int(0)
*** Testing re2_match(): no subpatterns, positive ANCHOR_START
int(1)
*** Testing re2_match(): no subpatterns, negative ANCHOR_START
int(0)
*** Testing re2_match(): subpattern used but no matches arguments passed
int(1)
*** Testing re2_match(): offset
int(1)
array(1) {
  [0]=>
  string(5) "regex"
}
*** Testing re2_match(): 1 subpattern
int(1)
array(2) {
  [0]=>
  string(17) "Hello regex world"
  [1]=>
  string(5) "regex"
}
*** Testing re2_match(): 3 subpatterns
int(1)
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
int(1)
array(6) {
  [0]=>
  string(17) "Hello regex world"
  ["foo"]=>
  string(5) "Hello"
  [1]=>
  string(5) "Hello"
  [2]=>
  string(5) "regex"
  ["bar"]=>
  string(5) "world"
  [3]=>
  string(5) "world"
}
*** Testing re2_match(): 1 subpattern & offset capture
int(1)
array(2) {
  [0]=>
  array(2) {
    [0]=>
    string(17) "Hello regex world"
    [1]=>
    int(0)
  }
  [1]=>
  array(2) {
    [0]=>
    string(5) "regex"
    [1]=>
    int(6)
  }
}
*** Testing re2_match(): offset & offset capture
int(0)
array(2) {
  [0]=>
  array(2) {
    [0]=>
    string(17) "Hello regex world"
    [1]=>
    int(0)
  }
  [1]=>
  array(2) {
    [0]=>
    string(5) "regex"
    [1]=>
    int(6)
  }
}
*** Testing re2_match(): named groups & offset capture
int(1)
array(6) {
  [0]=>
  array(2) {
    [0]=>
    string(17) "Hello regex world"
    [1]=>
    int(0)
  }
  ["foo"]=>
  array(2) {
    [0]=>
    string(5) "Hello"
    [1]=>
    int(0)
  }
  [1]=>
  array(2) {
    [0]=>
    string(5) "Hello"
    [1]=>
    int(0)
  }
  [2]=>
  array(2) {
    [0]=>
    string(5) "regex"
    [1]=>
    int(6)
  }
  ["bar"]=>
  array(2) {
    [0]=>
    string(5) "world"
    [1]=>
    int(12)
  }
  [3]=>
  array(2) {
    [0]=>
    string(5) "world"
    [1]=>
    int(12)
  }
}
*** Testing re2_match(): absent subpattern
int(1)
array(3) {
  [0]=>
  string(6) "barbaz"
  [1]=>
  string(0) ""
  [2]=>
  string(3) "bar"
}
*** Testing re2_match(): absent subpattern, zero width subpattern & offset capture
int(1)
array(4) {
  [0]=>
  array(2) {
    [0]=>
    string(6) "barbaz"
    [1]=>
    int(0)
  }
  [1]=>
  array(2) {
    [0]=>
    string(0) ""
    [1]=>
    int(-1)
  }
  [2]=>
  array(2) {
    [0]=>
    string(3) "bar"
    [1]=>
    int(0)
  }
  [3]=>
  array(2) {
    [0]=>
    string(0) ""
    [1]=>
    int(3)
  }
}
*** Testing re2_match(): bad flags
int(1)
array(2) {
  [0]=>
  string(17) "Hello regex world"
  [1]=>
  string(5) "regex"
}
*** Testing re2_match(): >10 subpatterns
int(1)
array(14) {
  [0]=>
  string(19) "Hello abcdefghijklm"
  [1]=>
  string(1) "a"
  [2]=>
  string(1) "b"
  [3]=>
  string(1) "c"
  [4]=>
  string(1) "d"
  [5]=>
  string(1) "e"
  [6]=>
  string(1) "f"
  [7]=>
  string(1) "g"
  [8]=>
  string(1) "h"
  [9]=>
  string(1) "i"
  [10]=>
  string(1) "j"
  [11]=>
  string(1) "k"
  [12]=>
  string(1) "l"
  [13]=>
  string(1) "m"
}
