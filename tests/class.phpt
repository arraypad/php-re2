--TEST--
re2 - class
--SKIPIF--
<?php include('skipif.inc'); ?>
--FILE--
<?php

$re2 = new RE2('Hello (\w+)');
var_dump($re2);

echo "*** Testing RE2 class: getPattern()\n";
var_dump($re2->getPattern());

echo "*** Testing RE2 class: __toString()\n";
var_dump((string)$re2);

echo "*** Testing RE2 class: match\n";
var_dump(re2_match($re2, 'Hello regex world', $matches), $matches);

echo "*** Testing RE2 class: match_all\n";
$matches = null;
var_dump(re2_match_all($re2, 'Hello regex Hello PHP', $matches), $matches);

echo "*** Testing RE2 class: replace\n";
var_dump(re2_replace($re2, 'Goodbye \1', 'Hello regex world'));

echo "*** Testing RE2 class: clone RE2\n";
$matches = null;
$newRe = clone $re2;
var_dump(re2_match($newRe, 'Hello regex world', $matches), $matches);

echo "*** Testing RE2 class: re-use RE2_Options\n";
$matches = null;
$options = $re2->getOptions();
$newRe = new RE2('[\w ]+', $options);
var_dump(re2_match($newRe, 'Hello regex world', $matches), $matches);

echo "*** Testing RE2 class: clone RE2_Options\n";
$matches = null;
$options = clone $re2->getOptions();
$newRe = new RE2('[\w ]+', $options);
var_dump(re2_match($newRe, 'Hello regex world', $matches), $matches);

echo "*** Testing RE2 class: change RE2_Options\n";
$matches = null;
$options = $re2->getOptions();
$options->setPosixSyntax(true);
$newRe = new RE2('[\w ]+', $options);
var_dump(re2_match($newRe, 'Hello regex world', $matches), $matches);
var_dump(re2_match($re2, 'Hello regex world', $matches), $matches);

echo "*** Testing RE2 class: clone and change RE2_Options\n";
$matches = null;
$options = clone $re2->getOptions();
$options->setPosixSyntax(true);
$newRe = new RE2('[\w ]+', $options);
var_dump(re2_match($newRe, 'Hello regex world', $matches), $matches);
var_dump(re2_match($re2, 'Hello regex world', $matches), $matches);

echo "*** Testing RE2_Set class\n";
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

    $s->Match("four zero one two three five", $matches );
    print_r($matches);

    $s->Match("five four three two one zero", $matches );
    print_r($matches);
}

?>
--EXPECTF--
object(RE2)#1 (1) {
  ["options":protected]=>
  object(RE2_Options)#2 (0) {
  }
}
*** Testing RE2 class: getPattern()
string(11) "Hello (\w+)"
*** Testing RE2 class: __toString()
string(11) "Hello (\w+)"
*** Testing RE2 class: match
int(1)
array(2) {
  [0]=>
  string(11) "Hello regex"
  [1]=>
  string(5) "regex"
}
*** Testing RE2 class: match_all
int(2)
array(2) {
  [0]=>
  array(2) {
    [0]=>
    string(11) "Hello regex"
    [1]=>
    string(9) "Hello PHP"
  }
  [1]=>
  array(2) {
    [0]=>
    string(5) "regex"
    [1]=>
    string(3) "PHP"
  }
}
*** Testing RE2 class: replace
string(19) "Goodbye regex world"
*** Testing RE2 class: clone RE2
int(1)
array(2) {
  [0]=>
  string(11) "Hello regex"
  [1]=>
  string(5) "regex"
}
*** Testing RE2 class: re-use RE2_Options
int(1)
array(1) {
  [0]=>
  string(17) "Hello regex world"
}
*** Testing RE2 class: clone RE2_Options
int(1)
array(1) {
  [0]=>
  string(17) "Hello regex world"
}
*** Testing RE2 class: change RE2_Options
re2/re2.cc:%d: Error parsing '[\w ]+': invalid escape sequence: \w

Warning: re2_match(): Invalid pattern in %s/class.php on line %d
bool(false)
NULL
int(1)
array(2) {
  [0]=>
  string(11) "Hello regex"
  [1]=>
  string(5) "regex"
}
*** Testing RE2 class: clone and change RE2_Options
re2/re2.cc:%d: Error parsing '[\w ]+': invalid escape sequence: \w

Warning: re2_match(): Invalid pattern in %s/class.php on line %d
bool(false)
NULL
int(1)
array(2) {
  [0]=>
  string(11) "Hello regex"
  [1]=>
  string(5) "regex"
}
*** Testing RE2_Set class
Compile ok
Array
(
    [0] => 4
    [1] => 0
    [2] => 1
    [3] => 2
    [4] => 3
    [5] => 5
)
Array
(
    [0] => 5
    [1] => 4
    [2] => 3
    [3] => 2
    [4] => 1
    [5] => 0
)

