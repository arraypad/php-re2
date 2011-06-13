--TEST--
re2 - re2_match
--FILE--
<?php

echo "*** Testing re2_match(): no subpatterns, positive match\n";
var_dump(re2_match('Hello \w+ world', 'Hello regex world'));

echo "*** Testing re2_match(): no subpatterns, negative match\n";
var_dump(re2_match('Hello \w+ world', 'Hello "regex" world'));

echo "*** Testing re2_match(): 1 subpattern\n";
var_dump(re2_match('Hello (\w+) world', 'Hello regex world', 1, $matches), $matches);
unset($matches);

echo "*** Testing re2_match(): 3 subpatterns\n";
var_dump(re2_match('(Hello) (\w+) (.*)', 'Hello regex world', 3, $matches), $matches);

?>
--EXPECTF--
*** Testing re2_match(): no subpatterns, positive match
bool(true)
*** Testing re2_match(): no subpatterns, negative match
bool(false)
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
