--TEST--
Metal compute — add two arrays on GPU
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

$library = $device->createLibraryWithSource(<<<'MSL'
#include <metal_stdlib>
using namespace metal;
kernel void add(
    device const float* a [[buffer(0)]],
    device const float* b [[buffer(1)]],
    device float* out     [[buffer(2)]],
    uint id [[thread_position_in_grid]])
{
    out[id] = a[id] + b[id];
}
MSL);

$fn = $library->getFunction('add');
$pipeline = $device->createComputePipelineState($fn);

var_dump($pipeline instanceof Metal\ComputePipelineState);
var_dump($pipeline->getMaxTotalThreadsPerThreadgroup() > 0);
var_dump($pipeline->getThreadExecutionWidth() > 0);

$count = 256;
$a = array_map('floatval', range(0, $count - 1));
$b = array_map('floatval', range($count, 2 * $count - 1));

$bufA = $device->createBufferFromData($a);
$bufB = $device->createBufferFromData($b);
$bufOut = $device->createBuffer($count * 4);

$queue = $device->createCommandQueue();
$cmd = $queue->createCommandBuffer();
$enc = $cmd->createComputeCommandEncoder();

$enc->setComputePipelineState($pipeline);
$enc->setBuffer($bufA, 0, 0);
$enc->setBuffer($bufB, 0, 1);
$enc->setBuffer($bufOut, 0, 2);

$tgSize = min($pipeline->getMaxTotalThreadsPerThreadgroup(), $count);
$enc->dispatchThreads([$count, 1, 1], [$tgSize, 1, 1]);
$enc->endEncoding();

$cmd->commit();
$cmd->waitUntilCompleted();

var_dump($cmd->getStatus() === Metal\CommandBufferStatusCompleted);

$results = $bufOut->getContents(Metal\FLOAT, $count);

// Verify all results
$allCorrect = true;
for ($i = 0; $i < $count; $i++) {
    $expected = $a[$i] + $b[$i];
    if (abs($results[$i] - $expected) > 0.001) {
        echo "FAIL at index $i: got {$results[$i]}, expected $expected\n";
        $allCorrect = false;
        break;
    }
}
var_dump($allCorrect);

echo "OK\n";
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
OK
