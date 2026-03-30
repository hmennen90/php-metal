--TEST--
Metal\Buffer — bounds checking
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

// Zero length buffer
try {
    $device->createBuffer(0);
    echo "FAIL: no exception\n";
} catch (Metal\Exception $e) {
    echo "OK: " . $e->getMessage() . "\n";
}

// Read beyond bounds
$buf = $device->createBuffer(16);
try {
    $buf->getContents(Metal\FLOAT, 100);
    echo "FAIL: no exception\n";
} catch (Metal\Exception $e) {
    echo "OK: " . $e->getMessage() . "\n";
}

// Write beyond bounds
try {
    $buf->writeContents([1.0, 2.0, 3.0, 4.0, 5.0], Metal\FLOAT);
    echo "FAIL: no exception\n";
} catch (Metal\Exception $e) {
    echo "OK: " . $e->getMessage() . "\n";
}

// Empty data
try {
    $device->createBufferFromData([], Metal\FLOAT);
    echo "FAIL: no exception\n";
} catch (Metal\Exception $e) {
    echo "OK: " . $e->getMessage() . "\n";
}
?>
--EXPECT--
OK: Buffer length must be positive
OK: Read would exceed buffer bounds
OK: Write would exceed buffer bounds
OK: Data array must not be empty
