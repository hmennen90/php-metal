--TEST--
Metal\BlitCommandEncoder — buffer copy
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

$src = $device->createBufferFromData([1.0, 2.0, 3.0, 4.0]);
$dst = $device->createBuffer(16);

$queue = $device->createCommandQueue();
$cmd = $queue->createCommandBuffer();
$blit = $cmd->createBlitCommandEncoder();

$blit->copyFromBuffer($src, 0, $dst, 0, 16);
$blit->endEncoding();

$cmd->commit();
$cmd->waitUntilCompleted();

$result = $dst->getContents(Metal\FLOAT, 4);
var_dump(abs($result[0] - 1.0) < 0.001);
var_dump(abs($result[3] - 4.0) < 0.001);

// Partial copy with offset
$dst2 = $device->createBuffer(16);
$cmd2 = $queue->createCommandBuffer();
$blit2 = $cmd2->createBlitCommandEncoder();
$blit2->copyFromBuffer($src, 4, $dst2, 8, 4); // copy 1 float from offset 4 to offset 8
$blit2->endEncoding();
$cmd2->commit();
$cmd2->waitUntilCompleted();

$result2 = $dst2->getContents(Metal\FLOAT, 3);
var_dump(abs($result2[2] - 2.0) < 0.001); // The copied float

echo "OK\n";
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
OK
