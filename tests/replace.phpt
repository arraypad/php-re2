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
