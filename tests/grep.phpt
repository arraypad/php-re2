--TEST--
re2 - grep
--FILE--
<?php

$re2 = new RE2('Hello (\w+)');

$input = array('Hello regex world', 'Hello php world', 'foo' => 'Goodbye world', 'Hello!', 'bar' => 'Hello cpp');

echo "*** Testing RE2 grep: positive\n";
print_r(re2_grep($re2, $input));

echo "*** Testing RE2 grep: negative\n";
print_r(re2_grep($re2, $input, RE2_GREP_INVERT));

echo "*** Testing RE2 grep: full positive\n";
print_r(re2_grep($re2, $input, RE2_MATCH_FULL));

echo "*** Testing RE2 grep: empty input\n";
print_r(re2_grep($re2, array()));

echo "*** Testing RE2 grep: empty output\n";
print_r(re2_grep('non-matching', $input));

echo "*** Testing RE2 grep: invalid input\n";
print_r(re2_grep($re2, "foobar"));

?>
--EXPECTF--
*** Testing RE2 grep: positive
Array
(
    [0] => Hello regex world
    [1] => Hello php world
    [bar] => Hello cpp
)
*** Testing RE2 grep: negative
Array
(
    [foo] => Goodbye world
    [2] => Hello!
)
*** Testing RE2 grep: full positive
Array
(
    [bar] => Hello cpp
)
*** Testing RE2 grep: empty input
Array
(
)
*** Testing RE2 grep: empty output
Array
(
)
*** Testing RE2 grep: invalid input

Warning: re2_grep() expects parameter 2 to be array, string given in %s on line %d
