--TEST--
re2 - grep
--FILE--
<?php

$re2 = new RE2('Hello (\w+)');

$input = array('Hello regex world', 'Hello php world', 'foo' => 'Goodbye world', 'Hello!', 'bar' => 'Hello cpp');

echo "*** Testing RE2 class: grep positive\n";
print_r(re2_grep($re2, $input));

echo "*** Testing RE2 class: grep negative\n";
print_r(re2_grep($re2, $input, RE2_GREP_INVERT));

echo "*** Testing RE2 class: grep full positive\n";
print_r(re2_grep($re2, $input, RE2_MATCH_FULL));

?>
--EXPECTF--
*** Testing RE2 class: grep positive
Array
(
    [0] => Hello regex world
    [1] => Hello php world
    [bar] => Hello cpp
)
*** Testing RE2 class: grep negative
Array
(
    [foo] => Goodbye world
    [2] => Hello!
)
*** Testing RE2 class: grep full positive
Array
(
    [bar] => Hello cpp
)
