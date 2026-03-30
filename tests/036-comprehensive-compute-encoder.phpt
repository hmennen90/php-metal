--TEST--
Metal ComputeCommandEncoder — comprehensive method coverage
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

$lib = $device->createLibraryWithSource("
#include <metal_stdlib>
kernel void k(device float* d [[buffer(0)]], uint i [[thread_position_in_grid]]) { d[i] = float(i); }
");
$pso = $device->createComputePipelineState($lib->getFunction('k'));

$buf = $device->createBuffer(256);
$queue = $device->createCommandQueue();

// Test all compute encoder methods
$cmd = $queue->createCommandBuffer();
$enc = $cmd->createComputeCommandEncoder();

$enc->setComputePipelineState($pso);
$enc->setBuffer($buf, 0, 0);
$enc->setBytes(pack('f', 3.14), 1);
$enc->setThreadgroupMemoryLength(64, 0);

// useResource
$enc->useResource($buf, Metal\ResourceUsageRead | Metal\ResourceUsageWrite);

// memoryBarrier
$enc->memoryBarrierWithScope(Metal\BarrierScopeBuffers);

// Fence operations
$fence = $device->createFence();
$enc->updateFence($fence);
$enc->waitForFence($fence);

// Debug groups
$enc->pushDebugGroup('compute-test');
$enc->popDebugGroup();

// Dispatch
$enc->dispatchThreads([64, 1, 1], [64, 1, 1]);
$enc->endEncoding();
$cmd->commit();
$cmd->waitUntilCompleted();

$r = $buf->getContents(Metal\FLOAT, 3);
var_dump(abs($r[0] - 0.0) < 0.001);
var_dump(abs($r[1] - 1.0) < 0.001);
var_dump(abs($r[2] - 2.0) < 0.001);

echo "OK\n";
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
OK
