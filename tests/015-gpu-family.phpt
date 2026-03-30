--TEST--
Metal\Device — GPU family support
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

// Apple Silicon should support at least Apple1
var_dump($device->supportsFamily(Metal\GPUFamilyApple1));
var_dump($device->supportsFamily(Metal\GPUFamilyCommon1));

// Should not support an absurdly high family
var_dump($device->supportsFamily(999999));

echo "OK\n";
?>
--EXPECT--
bool(true)
bool(true)
bool(false)
OK
