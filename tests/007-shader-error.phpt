--TEST--
Metal\Library — shader compilation error
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

try {
    $device->createLibraryWithSource("this is not valid MSL code!!!");
    echo "FAIL: no exception\n";
} catch (Metal\Exception $e) {
    echo "Caught: shader compilation failed\n";
    var_dump(str_contains($e->getMessage(), 'compilation failed'));
}

// Function not found
$lib = $device->createLibraryWithSource("
#include <metal_stdlib>
kernel void real_fn(device float* d [[buffer(0)]], uint id [[thread_position_in_grid]]) { d[id] = 0; }
");

try {
    $lib->getFunction('nonexistent');
    echo "FAIL: no exception\n";
} catch (Metal\Exception $e) {
    echo "Caught: function not found\n";
    var_dump(str_contains($e->getMessage(), 'not found'));
}
?>
--EXPECT--
Caught: shader compilation failed
bool(true)
Caught: function not found
bool(true)
