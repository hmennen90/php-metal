--TEST--
Metal ComputePassDescriptor and event encoding
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();
$queue = $device->createCommandQueue();

// ComputePassDescriptor
$cpd = new Metal\ComputePassDescriptor();
$cpd->setDispatchType(Metal\DispatchTypeSerial);
var_dump($cpd instanceof Metal\ComputePassDescriptor);

// Create compute encoder with descriptor
$lib = $device->createLibraryWithSource("
#include <metal_stdlib>
kernel void noop(device float* d [[buffer(0)]], uint id [[thread_position_in_grid]]) { d[id] = 1.0; }
");
$pipeline = $device->createComputePipelineState($lib->getFunction('noop'));
$buf = $device->createBuffer(40);

$cmd = $queue->createCommandBuffer();
$enc = $cmd->createComputeCommandEncoderWithDescriptor($cpd);
$enc->setComputePipelineState($pipeline);
$enc->setBuffer($buf, 0, 0);
$enc->dispatchThreads([10, 1, 1], [10, 1, 1]);
$enc->endEncoding();
$cmd->commit();
$cmd->waitUntilCompleted();

$results = $buf->getContents(Metal\FLOAT, 10);
var_dump(abs($results[0] - 1.0) < 0.001);
var_dump(abs($results[9] - 1.0) < 0.001);

// Event encoding
$event = $device->createSharedEvent();
$cmd2 = $queue->createCommandBuffer();
$enc2 = $cmd2->createComputeCommandEncoder();
$enc2->setComputePipelineState($pipeline);
$enc2->setBuffer($buf, 0, 0);
$enc2->dispatchThreads([10, 1, 1], [10, 1, 1]);
$enc2->endEncoding();
$cmd2->encodeSignalEvent($event, 1);
$cmd2->commit();
$cmd2->waitUntilCompleted();
var_dump($event->getSignaledValue() === 1);

echo "OK\n";
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
OK
