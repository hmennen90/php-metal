--TEST--
Metal compute — setBytes for scalar parameters
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

$library = $device->createLibraryWithSource(<<<'MSL'
#include <metal_stdlib>
using namespace metal;
kernel void scale(
    device const float* input [[buffer(0)]],
    device float* output      [[buffer(1)]],
    constant float& factor    [[buffer(2)]],
    uint id [[thread_position_in_grid]])
{
    output[id] = input[id] * factor;
}
MSL);

$pipeline = $device->createComputePipelineState($library->getFunction('scale'));

$input = [1.0, 2.0, 3.0, 4.0];
$bufIn = $device->createBufferFromData($input);
$bufOut = $device->createBuffer(count($input) * 4);

$queue = $device->createCommandQueue();
$cmd = $queue->createCommandBuffer();
$enc = $cmd->createComputeCommandEncoder();

$enc->setComputePipelineState($pipeline);
$enc->setBuffer($bufIn, 0, 0);
$enc->setBuffer($bufOut, 0, 1);
$enc->setBytes(pack('f', 10.0), 2);
$enc->dispatchThreads([count($input), 1, 1], [count($input), 1, 1]);
$enc->endEncoding();

$cmd->commit();
$cmd->waitUntilCompleted();

$output = $bufOut->getContents(Metal\FLOAT, count($input));
var_dump(abs($output[0] - 10.0) < 0.001);
var_dump(abs($output[1] - 20.0) < 0.001);
var_dump(abs($output[2] - 30.0) < 0.001);
var_dump(abs($output[3] - 40.0) < 0.001);

echo "OK\n";
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
OK
