--TEST--
re2 - re2_filter
--FILE--
<?php

echo "*** Testing re2_filter(): basic\n";
var_dump(re2_filter('Hello (\w+)', 'Goodbye \1', 'Hello regex world'));

echo "*** Testing re2_filter(): \\0 capture\n";
var_dump(re2_filter('Hello (\w+)', 'Again, \0', 'Hello regex world'));

echo "*** Testing re2_filter(): non matching string\n";
var_dump(re2_filter('Hello (\w+)', 'Goodbye \1', 'Hi regex world'));

echo "*** Testing re2_filter(): subject array 1/1\n";
var_dump(re2_filter('a', 'A', array('abc')));

echo "*** Testing re2_filter(): subject array 0/1\n";
var_dump(re2_filter('a', 'A', array('xyz')));

echo "*** Testing re2_filter(): subject array 2/3\n";
var_dump(re2_filter('a', 'A', array('abc', 'cba', 'xyz')));

echo "*** Testing re2_filter(): pattern array\n";
var_dump(re2_filter(array('a'), 'A', 'abc'));

echo "*** Testing re2_filter(): pattern array & replace string\n";
var_dump(re2_filter(array('a', 'b'), 'A', 'abc'));

echo "*** Testing re2_filter(): non matching pattern array & replace string\n";
var_dump(re2_filter(array('x', 'y'), 'A', 'abc'));

echo "*** Testing re2_filter(): non matching pattern array & replace array\n";
var_dump(re2_filter(array('x', 'y'), array('A', 'B'), 'abc'));

echo "*** Testing re2_filter(): pattern array & replace array & limit\n";
var_dump(re2_filter(array('a', 'b'), array('A', 'B'), 'aabc', 1));
echo "*** Testing re2_filter(): pattern array & replace array & subject array 2/4 & count\n";
var_dump(re2_filter(array('a', 'b'), array('A', 'B'), array('abc', 'cba', 'xyz', 'zyx'), -1, $count), $count);

?>
--EXPECTF--
*** Testing re2_filter(): basic
string(19) "Goodbye regex world"
*** Testing re2_filter(): \0 capture
string(24) "Again, Hello regex world"
*** Testing re2_filter(): non matching string
NULL
*** Testing re2_filter(): subject array 1/1
array(1) {
  [0]=>
  string(3) "Abc"
}
*** Testing re2_filter(): subject array 0/1
array(0) {
}
*** Testing re2_filter(): subject array 2/3
array(2) {
  [0]=>
  string(3) "Abc"
  [1]=>
  string(3) "cbA"
}
*** Testing re2_filter(): pattern array
string(3) "Abc"
*** Testing re2_filter(): pattern array & replace string
string(3) "AAc"
*** Testing re2_filter(): non matching pattern array & replace string
NULL
*** Testing re2_filter(): non matching pattern array & replace array
NULL
*** Testing re2_filter(): pattern array & replace array & limit
string(4) "AaBc"
*** Testing re2_filter(): pattern array & replace array & subject array 2/4 & count
array(2) {
  [0]=>
  string(3) "ABc"
  [1]=>
  string(3) "cBA"
}
int(4)
