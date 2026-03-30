--TEST--
Metal compute — dispatchThreadgroups
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

$library = $device->createLibraryWithSource(<<<'MSL'
#include <metal_stdlib>
using namespace metal;
kernel void fill(device float* out [[buffer(0)]], uint id [[thread_position_in_grid]]) {
    out[id] = float(id);
}
MSL);

$pipeline = $device->createComputePipelineState($library->getFunction('fill'));

$count = 1024;
$bufOut = $device->createBuffer($count * 4);

$queue = $device->createCommandQueue();
$cmd = $queue->createCommandBuffer();
$enc = $cmd->createComputeCommandEncoder();

$enc->setComputePipelineState($pipeline);
$enc->setBuffer($bufOut, 0, 0);

// Use dispatchThreadgroups instead of dispatchThreads
$threadsPerGroup = 64;
$numGroups = $count / $threadsPerGroup;
$enc->dispatchThreadgroups([$numGroups, 1, 1], [$threadsPerGroup, 1, 1]);
$enc->endEncoding();

$cmd->commit();
$cmd->waitUntilCompleted();

$results = $bufOut->getContents(Metal\FLOAT, 5);
var_dump(abs($results[0] - 0.0) < 0.001);
var_dump(abs($results[1] - 1.0) < 0.001);
var_dump(abs($results[4] - 4.0) < 0.001);

// Check last few
$last = $bufOut->getContents(Metal\FLOAT, 3, ($count - 3) * 4);
var_dump(abs($last[0] - 1021.0) < 0.001);
var_dump(abs($last[2] - 1023.0) < 0.001);

echo "OK\n";
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
OK
