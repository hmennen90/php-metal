--TEST--
Metal ComputePipelineDescriptor and Device::createComputePipelineStateWithDescriptor
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();
$lib = $device->createLibraryWithSource("
#include <metal_stdlib>
kernel void k(device float* d [[buffer(0)]], uint i [[thread_position_in_grid]]) { d[i] = 1.0; }
");

$desc = new Metal\ComputePipelineDescriptor();
var_dump($desc instanceof Metal\ComputePipelineDescriptor);

$desc->setComputeFunction($lib->getFunction('k'));
$desc->setThreadGroupSizeIsMultipleOfThreadExecutionWidth(false);
$desc->setMaxTotalThreadsPerThreadgroup(512);
$desc->setMaxCallStackDepth(1);
$desc->setSupportIndirectCommandBuffers(false);
$desc->setLabel('test-pipeline');

$pso = $device->createComputePipelineStateWithDescriptor($desc);
var_dump($pso instanceof Metal\ComputePipelineState);
var_dump($pso->getMaxTotalThreadsPerThreadgroup() > 0);

// Use it
$buf = $device->createBuffer(40);
$queue = $device->createCommandQueue();
$cmd = $queue->createCommandBuffer();
$enc = $cmd->createComputeCommandEncoder();
$enc->setComputePipelineState($pso);
$enc->setBuffer($buf, 0, 0);
$enc->dispatchThreads([10, 1, 1], [10, 1, 1]);
$enc->endEncoding();
$cmd->commit();
$cmd->waitUntilCompleted();

$r = $buf->getContents(Metal\FLOAT, 10);
var_dump(abs($r[0] - 1.0) < 0.001);

echo "OK\n";
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
OK
