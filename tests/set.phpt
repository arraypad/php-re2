--TEST--
re2 - set
--SKIPIF--
<?php include('skipif.inc'); ?>
--FILE--
<?php

echo "*** Testing Re2Set class: correct\n";
$s = new Re2Set();

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

echo "*** Testing Re2Set class: incorrect - not compiled\n";
$matches = null;
$s = new Re2Set();
$s->add('one');
try {
	var_dump($s->match('one', $matches), $matches);
} catch (Re2IllegalStateException $e) {
	echo get_class($e), ": ", $e->getMessage(), "\n";
}

echo "*** Testing Re2Set class: incorrect - no patterns\n";
$matches = null;
$s = new Re2Set();
try {
	var_dump($s->compile());
} catch (Re2IllegalStateException $e) {
	echo get_class($e), ": ", $e->getMessage(), "\n";
}

echo "*** Testing Re2Set class: incorrect - invalid pattern\n";
$matches = null;
$s = new Re2Set();
try {
	var_dump($s->add('?'));
} catch (Re2InvalidPatternException $e) {
	echo get_class($e), ": ", $e->getMessage(), "\n";
}

echo "*** Testing Re2Set class: correct - with options\n";
$matches = null;
$o = new Re2Options;
$o->setCaseSensitive(false);
$s = new Re2Set($o);
$s->add('foo');
$s->compile();
var_dump($s->match('FOO', $matches), $matches);

?>
--EXPECTF--
*** Testing Re2Set class: correct
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
*** Testing Re2Set class: incorrect - not compiled
Re2IllegalStateException: Set is not compiled
*** Testing Re2Set class: incorrect - no patterns
Re2IllegalStateException: Set has no patterns
*** Testing Re2Set class: incorrect - invalid pattern
re2/set.cc:%d: Error parsing '?': no argument for repetition operator: ?
Re2InvalidPatternException: no argument for repetition operator: ?
*** Testing Re2Set class: correct - with options
bool(true)
array(1) {
  [0]=>
  int(0)
}
