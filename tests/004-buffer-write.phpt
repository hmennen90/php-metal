--TEST--
Metal\Buffer — writeContents and raw I/O
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

// writeContents
$buf = $device->createBuffer(32);
$buf->writeContents([10.0, 20.0, 30.0], Metal\FLOAT);
$result = $buf->getContents(Metal\FLOAT, 3);
var_dump(abs($result[0] - 10.0) < 0.001);
var_dump(abs($result[1] - 20.0) < 0.001);
var_dump(abs($result[2] - 30.0) < 0.001);

// Raw I/O
$raw = pack('f*', 1.5, 2.5);
$buf->writeRawContents($raw);
$readBack = $buf->getRawContents(8);
$unpacked = unpack('f2v', $readBack);
var_dump(abs($unpacked['v1'] - 1.5) < 0.001);
var_dump(abs($unpacked['v2'] - 2.5) < 0.001);

// Offset write
$buf2 = $device->createBuffer(32);
$buf2->writeContents([100.0], Metal\FLOAT, 8); // offset 8 bytes = skip 2 floats
$vals = $buf2->getContents(Metal\FLOAT, 3);
var_dump(abs($vals[2] - 100.0) < 0.001);

echo "OK\n";
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
OK
