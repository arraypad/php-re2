--TEST--
re2 - re2_replace
--FILE--
<?php

echo "*** Testing re2_replace(): basic\n";
var_dump(re2_replace('Hello (\w+)', 'Goodbye \1', 'Hello regex world'));

echo "*** Testing re2_replace(): \\0 capture\n";
var_dump(re2_replace('Hello (\w+)', 'Again, \0', 'Hello regex world'));

echo "*** Testing re2_replace(): non matching\n";
var_dump(re2_replace('Hello (\w+)', 'Goodbye \1', 'Hi regex world'));

echo "*** Testing re2_replace(): with count\n";
var_dump(re2_replace('\w+', 'foo', 'Hello regex world', -1, $count), $count);

echo "*** Testing re2_replace(): first with count\n";
var_dump(re2_replace('\w+', 'foo', 'Hello regex world', 1, $count), $count);

echo "*** Testing re2_replace(): limit 2 with count\n";
var_dump(re2_replace('\w+', 'foo', 'Hello regex world', 2, $count), $count);

echo "*** Testing re2_replace(): limit 2 no count\n";
var_dump(re2_replace('\w+', 'foo', 'Hello regex world', 2));

echo "*** Testing re2_replace(): subject separation\n";
$s = 'abc';
var_dump(re2_replace('a', 'A', $s), $s);

echo "*** Testing re2_replace(): subject array separation\n";
$s = array('abc');
var_dump(re2_replace('a', 'A', $s), $s);

echo "*** Testing re2_replace(): subject array\n";
var_dump(re2_replace('a', 'A', array('abc')));

echo "*** Testing re2_replace(): subject array 2\n";
var_dump(re2_replace('a', 'A', array('abc', 'cba')));

echo "*** Testing re2_replace(): subject array 2 weird keys\n";
var_dump(re2_replace('a', 'A', array('foo' => 'abc', 42 => 'cba')));

echo "*** Testing re2_replace(): pattern array\n";
var_dump(re2_replace(array('a'), 'A', 'abc'));

echo "*** Testing re2_replace(): pattern array & replace string\n";
var_dump(re2_replace(array('a', 'b'), 'A', 'abc'));

echo "*** Testing re2_replace(): pattern array & replace array\n";
var_dump(re2_replace(array('a', 'b'), array('A', 'B'), 'abc'));

echo "*** Testing re2_replace(): pattern array & replace array & limit\n";
var_dump(re2_replace(array('a', 'b'), array('A', 'B'), 'aabc', 1));

echo "*** Testing re2_replace(): pattern array & smaller replace array\n";
var_dump(re2_replace(array('a', 'b'), array('A'), 'abc'));

echo "*** Testing re2_replace(): pattern array & replace array & subject array\n";
var_dump(re2_replace(array('a', 'b'), array('A', 'B'), array('abc')));

echo "*** Testing re2_replace(): pattern array & replace array & subject array 2 & count\n";
var_dump(re2_replace(array('a', 'b'), array('A', 'B'), array('abc', 'cba'), -1, $count), $count);

?>
--EXPECTF--
*** Testing re2_replace(): basic
string(19) "Goodbye regex world"
*** Testing re2_replace(): \0 capture
string(24) "Again, Hello regex world"
*** Testing re2_replace(): non matching
string(14) "Hi regex world"
*** Testing re2_replace(): with count
string(11) "foo foo foo"
int(3)
*** Testing re2_replace(): first with count
string(15) "foo regex world"
int(1)
*** Testing re2_replace(): limit 2 with count
string(13) "foo foo world"
int(2)
*** Testing re2_replace(): limit 2 no count
string(13) "foo foo world"
*** Testing re2_replace(): subject separation
string(3) "Abc"
string(3) "abc"
*** Testing re2_replace(): subject array separation
array(1) {
  [0]=>
  string(3) "Abc"
}
array(1) {
  [0]=>
  string(3) "abc"
}
*** Testing re2_replace(): subject array
array(1) {
  [0]=>
  string(3) "Abc"
}
*** Testing re2_replace(): subject array 2
array(2) {
  [0]=>
  string(3) "Abc"
  [1]=>
  string(3) "cbA"
}
*** Testing re2_replace(): subject array 2 weird keys
array(2) {
  ["foo"]=>
  string(3) "Abc"
  [42]=>
  string(3) "cbA"
}
*** Testing re2_replace(): pattern array
string(3) "Abc"
*** Testing re2_replace(): pattern array & replace string
string(3) "AAc"
*** Testing re2_replace(): pattern array & replace array
string(3) "ABc"
*** Testing re2_replace(): pattern array & replace array & limit
string(4) "AaBc"
*** Testing re2_replace(): pattern array & smaller replace array
string(2) "Ac"
*** Testing re2_replace(): pattern array & replace array & subject array
array(1) {
  [0]=>
  string(3) "ABc"
}
*** Testing re2_replace(): pattern array & replace array & subject array 2 & count
array(2) {
  [0]=>
  string(3) "ABc"
  [1]=>
  string(3) "cBA"
}
int(4)
