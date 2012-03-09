--TEST--
re2 - cache with different options
--SKIPIF--
<?php include('skipif.inc'); ?>
--INI--
re2.cache_enabled=off
--FILE--
<?php

$opt1 = new Re2Options;
$opt1->setPosixSyntax(true);

$opt2 = new Re2Options;
$opt2->setPosixSyntax(false);

echo "*** Testing RE2 cache with different options\n";
try { var_dump(re2_match(new RE2('fo\w', $opt1, true), 'foo')); } catch (Exception $e) {}
try { var_dump(re2_match(new RE2('fo\w', $opt2, true), 'foo')); } catch (Exception $e) {}
try { var_dump(re2_match(new RE2('fo\w', $opt1, true), 'foo')); } catch (Exception $e) {}
try { var_dump(re2_match(new RE2('fo\w', $opt2, true), 'foo')); } catch (Exception $e) {}

?>
--EXPECTF--
*** Testing RE2 cache with different options
re2/re2.cc:%d: Error parsing 'fo\w': invalid escape sequence: \w
int(1)
re2/re2.cc:%d: Error parsing 'fo\w': invalid escape sequence: \w
int(1)
