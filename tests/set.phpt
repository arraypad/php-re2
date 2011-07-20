--TEST--
re2 - set
--SKIPIF--
<?php include('skipif.inc'); ?>
--FILE--
<?php

echo "*** Testing RE2_Set class: correct\n";
$s = new RE2_Set();

assert($s->Add("zero") == 0);
assert($s->Add("one") == 1);
assert($s->Add("two") == 2);
assert($s->Add("three") == 3);
assert($s->Add("four") == 4);
assert($s->Add("five") == 5);

if(!$s->Compile()) {
    print "Compile failed!\n";
}else{
    print "Compile ok\n";

    $matches = null;

	echo "* All matching: forwards\n";
    $s->Match("four zero one two three five", $matches );
    print_r($matches);

	echo "* All matching: backwards\n";
    $s->Match("five four three two one zero", $matches );
    print_r($matches);

	echo "* None matching\n";
    $s->Match("six seven eight", $matches );
    print_r($matches);
}

echo "*** Testing RE2_Set class: incorrect - not compiled\n";
$matches = null;
$s = new RE2_Set();
$s->add('one');
var_dump($s->match('one', $matches), $matches);

echo "*** Testing RE2_Set class: incorrect - no patterns\n";
$matches = null;
$s = new RE2_Set();
var_dump($s->compile());

?>
--EXPECTF--
*** Testing RE2_Set class: correct
Compile ok
* All matching: forwards
Array
(
    [0] => 4
    [1] => 0
    [2] => 1
    [3] => 2
    [4] => 3
    [5] => 5
)
* All matching: backwards
Array
(
    [0] => 5
    [1] => 4
    [2] => 3
    [3] => 2
    [4] => 1
    [5] => 0
)
* None matching
Array
(
)
*** Testing RE2_Set class: incorrect - not compiled

Warning: RE2_Set::match(): Set is not compiled in %s/set.php on line %d
bool(false)
NULL
*** Testing RE2_Set class: incorrect - no patterns

Warning: RE2_Set::compile(): Set has no patterns in %s/set.php on line %d
bool(false)
