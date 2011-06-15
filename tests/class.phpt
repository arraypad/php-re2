--TEST--
re2 - class
--FILE--
<?php

$re2 = new RE2('Hello (\w+)');
var_dump($re2);

echo "*** Testing RE2 class: match\n";
var_dump(re2_match($re2, 'Hello regex world', $matches), $matches);

echo "*** Testing RE2 class: match_all\n";
var_dump(re2_match_all($re2, 'Hello regex Hello PHP', $matches), $matches);

echo "*** Testing RE2 class: replace\n";
var_dump(re2_replace($re2, 'Goodbye \1', 'Hello regex world'));
?>
--EXPECTF--
object(RE2)#1 (1) {
  ["options":protected]=>
  object(RE2_Options)#2 (0) {
  }
}
*** Testing RE2 class: match
bool(true)
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
