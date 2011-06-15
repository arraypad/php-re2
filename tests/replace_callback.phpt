--TEST--
re2 - re2_replace_callback
--FILE--
<?php

function repl($m) {
  return $m[1] . strtoupper($m[2]);
}

echo "*** Testing re2_replace_callback(): basic\n";
var_dump(re2_replace_callback('(Hello )(\w+)', 'repl', 'Hello regex world'));


?>
--EXPECTF--
*** Testing re2_replace_callback(): basic
string(17) "Hello REGEX world"
