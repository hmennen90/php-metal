--TEST--
Metal Buffer enhanced methods
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

$buf = $device->createBufferFromData([1.0, 2.0, 3.0]);

// gpuAddress
$addr = $buf->getGpuAddress();
var_dump($addr > 0);

// didModifyRange (no-op on shared buffers, but should not crash)
$buf->didModifyRange(0, 12);

echo "OK\n";
?>
--EXPECT--
bool(true)
OK
