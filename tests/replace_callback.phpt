--TEST--
re2 - re2_replace_callback
--SKIPIF--
<?php include('skipif.inc'); ?>
--FILE--
<?php

function repl($m) {
  return $m[1] . strtoupper($m[2]);
}

echo "*** Testing re2_replace_callback(): basic\n";
var_dump(re2_replace_callback('(Hello )(\w+)', 'repl', 'Hello regex world'));

function repl2($m) {
  return strtoupper($m[0]);
}

echo "*** Testing re2_replace_callback(): subject array\n";
var_dump(re2_replace_callback('a', 'repl2', array('abc')));

echo "*** Testing re2_replace_callback(): subject array 2\n";
var_dump(re2_replace_callback('a', 'repl2', array('abc', 'cba')));

echo "*** Testing re2_replace_callback(): pattern array\n";
var_dump(re2_replace_callback(array('a'), 'repl2', 'abc'));

echo "*** Testing re2_replace_callback(): pattern array & subject array\n";
var_dump(re2_replace_callback(array('a', 'b'), 'repl2', array('abc')));

echo "*** Testing re2_replace_callback(): pattern array & subject array 2\n";
var_dump(re2_replace_callback(array('a', 'b'), 'repl2', array('abc', 'cba')));

?>
--EXPECTF--
*** Testing re2_replace_callback(): basic
string(17) "Hello REGEX world"
*** Testing re2_replace_callback(): subject array
array(1) {
  [0]=>
  string(3) "Abc"
}
*** Testing re2_replace_callback(): subject array 2
array(2) {
  [0]=>
  string(3) "Abc"
  [1]=>
  string(3) "cbA"
}
*** Testing re2_replace_callback(): pattern array
string(3) "Abc"
*** Testing re2_replace_callback(): pattern array & subject array
array(1) {
  [0]=>
  string(3) "ABc"
}
*** Testing re2_replace_callback(): pattern array & subject array 2
array(2) {
  [0]=>
  string(3) "ABc"
  [1]=>
  string(3) "cBA"
}
