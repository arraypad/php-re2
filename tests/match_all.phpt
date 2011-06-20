--TEST--
re2 - re2_match_all
--SKIPIF--
<?php include('skipif.inc'); ?>
--FILE--
<?php

echo "*** Testing re2_match_all(): 1 subpattern\n";
$subject = 'Hello regex world Hello php world Hello cpp world';
var_dump(re2_match_all('Hello (\w+) world', $subject, $matches)); print_r($matches);

echo "*** Testing re2_match_all(): 3 subpatterns\n";
var_dump(re2_match_all('(Hello) (\w+) (\w+)', $subject, $matches)); print_r($matches);

echo "*** Testing re2_match_all(): 3 subpatterns, RE2_SET_ORDER\n";
var_dump(re2_match_all('(Hello) (\w+) (\w+)', $subject, $matches, RE2_SET_ORDER)); print_r($matches);

echo "*** Testing re2_match_all(): offset\n";
var_dump(re2_match_all('Hello \w+ world', $subject, $matches, RE2_PATTERN_ORDER, 17)); print_r($matches);

echo "*** Testing re2_match_all(): offset capture\n";
var_dump(re2_match_all('Hello (\w+) world', $subject, $matches, RE2_OFFSET_CAPTURE)); print_r($matches);

echo "*** Testing re2_match_all(): offset capture, RE2_SET_ORDER\n";
var_dump(re2_match_all('Hello (\w+) world', $subject, $matches, RE2_OFFSET_CAPTURE | RE2_SET_ORDER)); print_r($matches);

echo "*** Testing re2_match_all(): end with empty capture\n";
var_dump(re2_match_all('Hello \w+ world|z*', $subject, $matches)); print_r($matches);

echo "*** Testing re2_match_all(): bad flags\n";
var_dump(re2_match_all('Hello (\w+) world', $subject, $matches, RE2_SPLIT_NO_EMPTY | RE2_GREP_INVERT | RE2_SET_ORDER | RE2_PATTERN_ORDER)); print_r($matches);

?>
--EXPECTF--
*** Testing re2_match_all(): 1 subpattern
int(3)
Array
(
    [0] => Array
        (
            [0] => Hello regex world
            [1] => Hello php world
            [2] => Hello cpp world
        )

    [1] => Array
        (
            [0] => regex
            [1] => php
            [2] => cpp
        )

)
*** Testing re2_match_all(): 3 subpatterns
int(3)
Array
(
    [0] => Array
        (
            [0] => Hello regex world
            [1] => Hello php world
            [2] => Hello cpp world
        )

    [1] => Array
        (
            [0] => Hello
            [1] => Hello
            [2] => Hello
        )

    [2] => Array
        (
            [0] => regex
            [1] => php
            [2] => cpp
        )

    [3] => Array
        (
            [0] => world
            [1] => world
            [2] => world
        )

)
*** Testing re2_match_all(): 3 subpatterns, RE2_SET_ORDER
int(3)
Array
(
    [0] => Array
        (
            [0] => Hello regex world
            [1] => Hello
            [2] => regex
            [3] => world
        )

    [1] => Array
        (
            [0] => Hello php world
            [1] => Hello
            [2] => php
            [3] => world
        )

    [2] => Array
        (
            [0] => Hello cpp world
            [1] => Hello
            [2] => cpp
            [3] => world
        )

)
*** Testing re2_match_all(): offset
int(2)
Array
(
    [0] => Array
        (
            [0] => Hello php world
            [1] => Hello cpp world
        )

)
*** Testing re2_match_all(): offset capture
int(3)
Array
(
    [0] => Array
        (
            [0] => Array
                (
                    [0] => Hello regex world
                    [1] => 0
                )

            [1] => Array
                (
                    [0] => Hello php world
                    [1] => 18
                )

            [2] => Array
                (
                    [0] => Hello cpp world
                    [1] => 34
                )

        )

    [1] => Array
        (
            [0] => Array
                (
                    [0] => regex
                    [1] => 6
                )

            [1] => Array
                (
                    [0] => php
                    [1] => 24
                )

            [2] => Array
                (
                    [0] => cpp
                    [1] => 40
                )

        )

)
*** Testing re2_match_all(): offset capture, RE2_SET_ORDER
int(3)
Array
(
    [0] => Array
        (
            [0] => Array
                (
                    [0] => Hello regex world
                    [1] => 0
                )

            [1] => Array
                (
                    [0] => regex
                    [1] => 6
                )

        )

    [1] => Array
        (
            [0] => Array
                (
                    [0] => Hello php world
                    [1] => 18
                )

            [1] => Array
                (
                    [0] => php
                    [1] => 24
                )

        )

    [2] => Array
        (
            [0] => Array
                (
                    [0] => Hello cpp world
                    [1] => 34
                )

            [1] => Array
                (
                    [0] => cpp
                    [1] => 40
                )

        )

)
*** Testing re2_match_all(): end with empty capture
int(5)
Array
(
    [0] => Array
        (
            [0] => Hello regex world
            [1] => 
            [2] => Hello php world
            [3] => 
            [4] => Hello cpp world
        )

)
*** Testing re2_match_all(): bad flags
int(3)
Array
(
    [0] => Array
        (
            [0] => Hello regex world
            [1] => Hello php world
            [2] => Hello cpp world
        )

    [1] => Array
        (
            [0] => regex
            [1] => php
            [2] => cpp
        )

)
