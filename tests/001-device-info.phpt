--TEST--
Metal\Device — basic device info
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

// Device must exist
var_dump($device instanceof Metal\Device);

// Name must be a non-empty string
$name = $device->getName();
var_dump(is_string($name) && strlen($name) > 0);

// Max buffer length must be positive
var_dump($device->getMaxBufferLength() > 0);

// Max threadgroup memory must be positive
var_dump($device->getMaxThreadgroupMemoryLength() > 0);

// Max threads per threadgroup returns [w, h, d]
$maxThreads = $device->getMaxThreadsPerThreadgroup();
var_dump(count($maxThreads) === 3);
var_dump($maxThreads[0] > 0);

// Registry ID
var_dump($device->getRegistryID() > 0);

// Bool properties
var_dump(is_bool($device->isLowPower()));
var_dump(is_bool($device->isRemovable()));

echo "OK\n";
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
OK
