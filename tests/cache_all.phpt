--TEST--
re2 - cache all
--SKIPIF--
<?php include('skipif.inc'); ?>
--INI--
re2.cache_enabled=on
--FILE--
<?php

echo "*** Testing RE2 cache all\n";
var_dump(re2_match('foo', 'foo'));
var_dump(re2_match('bar', 'bar'));
var_dump(re2_match('foo', 'foo'));
var_dump(re2_match('bar', 'bar'));

?>
--EXPECTF--
*** Testing RE2 cache all
int(1)
int(1)
int(1)
int(1)
