--TEST--
re2 - re2_grep
--SKIPIF--
<?php include('skipif.inc'); ?>
--FILE--
<?php

$re2 = new RE2('Hello (\w+)');

$input = array('Hello regex world', 'Hello php world', 'foo' => 'Goodbye world', 'Hello!', 'bar' => 'Hello cpp');

echo "*** Testing RE2 grep: positive\n";
print_r(re2_grep($re2, $input));

echo "*** Testing RE2 grep: negative\n";
print_r(re2_grep($re2, $input, RE2_GREP_INVERT));

echo "*** Testing RE2 grep: anchored positive\n";
print_r(re2_grep($re2, $input, RE2_ANCHOR_BOTH));

echo "*** Testing RE2 grep: anchored negative\n";
print_r(re2_grep($re2, $input, RE2_ANCHOR_BOTH | RE2_GREP_INVERT));

echo "*** Testing RE2 grep: empty input\n";
print_r(re2_grep($re2, array()));

echo "*** Testing RE2 grep: empty output\n";
print_r(re2_grep('non-matching', $input));

echo "*** Testing RE2 grep: invalid input\n";
print_r(re2_grep($re2, "foobar"));

echo "*** Testing RE2 grep: bad flags\n";
print_r(re2_grep($re2, $input, RE2_PATTERN_ORDER | RE2_SPLIT_NO_EMPTY));

echo "*** Testing RE2 grep: invalid pattern\n";
var_dump(re2_grep('\X+', array('Hello world42')));

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
*** Testing RE2 grep: anchored positive
Array
(
    [bar] => Hello cpp
)
*** Testing RE2 grep: anchored negative
Array
(
    [0] => Hello regex world
    [1] => Hello php world
    [foo] => Goodbye world
    [2] => Hello!
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
*** Testing RE2 grep: bad flags
Array
(
    [0] => Hello regex world
    [1] => Hello php world
    [bar] => Hello cpp
)
*** Testing RE2 grep: invalid pattern
re2/re2.cc:%d: Error parsing '\X+': invalid escape sequence: \X

Warning: re2_grep(): Invalid pattern in %s/grep.php on line %d
bool(false)
