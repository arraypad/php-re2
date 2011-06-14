--TEST--
re2 - re2_match_all
--FILE--
<?php

echo "*** Testing re2_match_all(): 1 subpattern\n";
$subject = 'Hello regex world Hello php world Hello cpp world';
var_dump(re2_match_all('Hello (\w+) world', $subject, 1, $matches), $matches);

echo "*** Testing re2_match_all(): 3 subpatterns\n";
var_dump(re2_match_all('(Hello) (\w+) (\w+)', $subject, 3, $matches), $matches);

?>
--EXPECTF--
*** Testing re2_match_all(): 1 subpattern
int(3)
array(3) {
  [0]=>
  array(1) {
    [0]=>
    string(5) "regex"
  }
  [1]=>
  array(1) {
    [0]=>
    string(3) "php"
  }
  [2]=>
  array(1) {
    [0]=>
    string(3) "cpp"
  }
}
*** Testing re2_match_all(): 3 subpatterns
int(3)
array(3) {
  [0]=>
  array(3) {
    [0]=>
    string(5) "Hello"
    [1]=>
    string(5) "regex"
    [2]=>
    string(5) "world"
  }
  [1]=>
  array(3) {
    [0]=>
    string(5) "Hello"
    [1]=>
    string(3) "php"
    [2]=>
    string(5) "world"
  }
  [2]=>
  array(3) {
    [0]=>
    string(5) "Hello"
    [1]=>
    string(3) "cpp"
    [2]=>
    string(5) "world"
  }
}
