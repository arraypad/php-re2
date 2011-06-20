--TEST--
re2 - re2_split
--SKIPIF--
<?php include('skipif.inc'); ?>
--FILE--
<?php

$re2 = new RE2('X');
$re2_group = new RE2('(X)');
$subject = 'fooXbarXXhelloXworld';

echo "*** Testing RE2 split: basic\n";
print_r(re2_split($re2, $subject));

echo "*** Testing RE2 split: limit 2\n";
print_r(re2_split($re2, $subject, 2));

echo "*** Testing RE2 split: no empty\n";
print_r(re2_split($re2, $subject, -1, RE2_SPLIT_NO_EMPTY));

echo "*** Testing RE2 split: no empty & offset capture\n";
print_r(re2_split($re2, $subject, -1, RE2_SPLIT_NO_EMPTY | RE2_SPLIT_OFFSET_CAPTURE));

echo "*** Testing RE2 split: delim capture without group\n";
print_r(re2_split($re2, $subject, -1, RE2_SPLIT_DELIM_CAPTURE));

echo "*** Testing RE2 split: delim capture with group\n";
print_r(re2_split($re2_group, $subject, -1, RE2_SPLIT_DELIM_CAPTURE));

echo "*** Testing RE2 split: delim capture with 2 groups\n";
print_r(re2_split('(X)x(X)', str_replace('X', 'XxX', $subject), -1, RE2_SPLIT_DELIM_CAPTURE));

echo "*** Testing RE2 split: delim capture & offset capture\n";
print_r(re2_split($re2_group, $subject, -1, RE2_SPLIT_DELIM_CAPTURE | RE2_SPLIT_OFFSET_CAPTURE));

echo "*** Testing RE2 split: offset capture\n";
print_r(re2_split($re2, $subject, -1, RE2_SPLIT_OFFSET_CAPTURE));

echo "*** Testing RE2 split: empty pattern\n";
print_r(re2_split('', 'hello'));

echo "*** Testing RE2 split: empty pattern & no empty\n";
print_r(re2_split('', 'hello', -1, RE2_SPLIT_NO_EMPTY));

echo "*** Testing RE2 split: partial empty pattern\n";
print_r(re2_split('XX|', 'fooXXbar'));

echo "*** Testing RE2 split: partial empty pattern with delim capture\n";
print_r(re2_split('(XX|)', 'fooXXbar', -1, RE2_SPLIT_DELIM_CAPTURE));

?>
--EXPECTF--
*** Testing RE2 split: basic
Array
(
    [0] => foo
    [1] => bar
    [2] => 
    [3] => hello
    [4] => world
)
*** Testing RE2 split: limit 2
Array
(
    [0] => foo
    [1] => barXXhelloXworld
)
*** Testing RE2 split: no empty
Array
(
    [0] => foo
    [1] => bar
    [2] => hello
    [3] => world
)
*** Testing RE2 split: no empty & offset capture
Array
(
    [0] => Array
        (
            [0] => foo
            [1] => 0
        )

    [1] => Array
        (
            [0] => bar
            [1] => 4
        )

    [2] => Array
        (
            [0] => hello
            [1] => 9
        )

    [3] => Array
        (
            [0] => world
            [1] => 15
        )

)
*** Testing RE2 split: delim capture without group
Array
(
    [0] => foo
    [1] => bar
    [2] => 
    [3] => hello
    [4] => world
)
*** Testing RE2 split: delim capture with group
Array
(
    [0] => foo
    [1] => X
    [2] => bar
    [3] => X
    [4] => 
    [5] => X
    [6] => hello
    [7] => X
    [8] => world
)
*** Testing RE2 split: delim capture with 2 groups
Array
(
    [0] => foo
    [1] => X
    [2] => X
    [3] => bar
    [4] => X
    [5] => X
    [6] => 
    [7] => X
    [8] => X
    [9] => hello
    [10] => X
    [11] => X
    [12] => world
)
*** Testing RE2 split: delim capture & offset capture
Array
(
    [0] => Array
        (
            [0] => foo
            [1] => 0
        )

    [1] => Array
        (
            [0] => X
            [1] => 0
        )

    [2] => Array
        (
            [0] => bar
            [1] => 4
        )

    [3] => Array
        (
            [0] => X
            [1] => 4
        )

    [4] => Array
        (
            [0] => 
            [1] => 8
        )

    [5] => Array
        (
            [0] => X
            [1] => 8
        )

    [6] => Array
        (
            [0] => hello
            [1] => 9
        )

    [7] => Array
        (
            [0] => X
            [1] => 9
        )

    [8] => Array
        (
            [0] => world
            [1] => 15
        )

)
*** Testing RE2 split: offset capture
Array
(
    [0] => Array
        (
            [0] => foo
            [1] => 0
        )

    [1] => Array
        (
            [0] => bar
            [1] => 4
        )

    [2] => Array
        (
            [0] => 
            [1] => 8
        )

    [3] => Array
        (
            [0] => hello
            [1] => 9
        )

    [4] => Array
        (
            [0] => world
            [1] => 15
        )

)
*** Testing RE2 split: empty pattern
Array
(
    [0] => 
    [1] => h
    [2] => 
    [3] => e
    [4] => 
    [5] => l
    [6] => 
    [7] => l
    [8] => 
    [9] => o
    [10] => 
)
*** Testing RE2 split: empty pattern & no empty
Array
(
    [0] => h
    [1] => e
    [2] => l
    [3] => l
    [4] => o
)
*** Testing RE2 split: partial empty pattern
Array
(
    [0] => 
    [1] => f
    [2] => 
    [3] => o
    [4] => 
    [5] => o
    [6] => XX
    [7] => b
    [8] => 
    [9] => a
    [10] => 
    [11] => r
    [12] => 
)
*** Testing RE2 split: partial empty pattern with delim capture
Array
(
    [0] => 
    [1] => 
    [2] => f
    [3] => 
    [4] => 
    [5] => o
    [6] => 
    [7] => 
    [8] => o
    [9] => 
    [10] => XX
    [11] => 
    [12] => b
    [13] => 
    [14] => 
    [15] => a
    [16] => 
    [17] => 
    [18] => r
    [19] => 
    [20] => 
)
