--TEST--
re2 - cache force
--SKIPIF--
<?php include('skipif.inc'); ?>
--INI--
re2.cache_enabled=off
--FILE--
<?php

echo "*** Testing RE2 cache force\n";
var_dump(re2_match(new RE2('foo', null, true), 'foo'));
var_dump(re2_match(new RE2('bar', null, true), 'bar'));
var_dump(re2_match(new RE2('foo', null, true), 'foo'));
var_dump(re2_match(new RE2('bar', null, true), 'bar'));

?>
--EXPECTF--
*** Testing RE2 cache force
int(1)
int(1)
int(1)
int(1)
