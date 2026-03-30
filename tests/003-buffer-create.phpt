--TEST--
Metal\Buffer — create and read/write typed data
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

// Create empty buffer
$buf = $device->createBuffer(1024);
var_dump($buf instanceof Metal\Buffer);
var_dump($buf->getLength() === 1024);

// Create buffer from float data
$data = [1.0, 2.0, 3.0, 4.0, 5.0];
$buf = $device->createBufferFromData($data, Metal\FLOAT);
var_dump($buf->getLength() === 20); // 5 * 4 bytes

$contents = $buf->getContents(Metal\FLOAT, 5);
var_dump(count($contents) === 5);
var_dump(abs($contents[0] - 1.0) < 0.001);
var_dump(abs($contents[4] - 5.0) < 0.001);

// Create buffer from int32 data
$intData = [10, 20, 30];
$intBuf = $device->createBufferFromData($intData, Metal\INT32);
var_dump($intBuf->getLength() === 12); // 3 * 4 bytes

$intContents = $intBuf->getContents(Metal\INT32, 3);
var_dump($intContents[0] === 10);
var_dump($intContents[2] === 30);

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
