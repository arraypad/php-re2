--TEST--
re2 - re2_quote
--FILE--
<?php

$pattern = 'Hello (\w+)';

echo "*** Testing re2_quote(): unquoted\n";
var_dump(re2_replace($pattern, 'foo', $pattern . ' Hello world'));

echo "*** Testing re2_quote(): quoted\n";
var_dump(re2_replace(re2_quote($pattern), 'foo', $pattern . ' Hello world'));

?>
--EXPECTF--
*** Testing re2_quote(): unquoted
string(15) "Hello (\w+) foo"
*** Testing re2_quote(): quoted
string(15) "foo Hello world"
