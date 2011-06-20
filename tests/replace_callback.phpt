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

function repl3($m) {
  return null;
}

echo "*** Testing re2_replace_callback(): return null\n";
var_dump(re2_replace_callback('(Hello )(\w+)', 'repl3', 'Hello regex world'));

function repl4($m) {
  return array();
}

echo "*** Testing re2_replace_callback(): return array\n";
var_dump(re2_replace_callback('(Hello )(\w+)', 'repl4', 'Hello regex world'));

function repl5($m) {
  return 3.14;
}

echo "*** Testing re2_replace_callback(): return number\n";
var_dump(re2_replace_callback('(Hello )(\w+)', 'repl5', 'Hello regex world'));

echo "*** Testing re2_replace_callback(): invalid callback\n";
var_dump(re2_replace_callback('(Hello )(\w+)', 'replX', 'Hello regex world'));

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
*** Testing re2_replace_callback(): return null
string(6) " world"
*** Testing re2_replace_callback(): return array

Notice: Array to string conversion in %s/replace_callback.php on line %d
string(11) "Array world"
*** Testing re2_replace_callback(): return number
string(10) "3.14 world"
*** Testing re2_replace_callback(): invalid callback

Warning: re2_replace_callback() expects parameter 2 to be a valid callback, function 'replX' not found or invalid function name in %s/replace_callback.php on line %d
bool(false)
