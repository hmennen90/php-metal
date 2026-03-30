<?php
/**
 * php-metal — GPU Compute Example
 *
 * Adds two arrays of floats on the GPU using a Metal compute shader.
 */

$device = Metal\createSystemDefaultDevice();

echo "GPU: " . $device->getName() . "\n";
echo "Max buffer: " . number_format($device->getMaxBufferLength() / 1024 / 1024) . " MB\n";
echo "Max threadgroup memory: " . number_format($device->getMaxThreadgroupMemoryLength()) . " bytes\n";
echo "Max threads/threadgroup: " . implode('x', $device->getMaxThreadsPerThreadgroup()) . "\n";
echo "\n";

// --- Shader source ---
$shaderSource = <<<'METAL'
#include <metal_stdlib>
using namespace metal;

kernel void add_arrays(
    device const float* a [[buffer(0)]],
    device const float* b [[buffer(1)]],
    device float* result   [[buffer(2)]],
    uint index [[thread_position_in_grid]])
{
    result[index] = a[index] + b[index];
}

kernel void multiply_scalar(
    device const float* input [[buffer(0)]],
    device float* output      [[buffer(1)]],
    constant float& scalar    [[buffer(2)]],
    uint index [[thread_position_in_grid]])
{
    output[index] = input[index] * scalar;
}
METAL;

// --- Compile shader ---
echo "Compiling shader... ";
$library = $device->createLibraryWithSource($shaderSource);
echo "OK\n";
echo "Functions: " . implode(', ', $library->getFunctionNames()) . "\n\n";

// --- Test 1: Add two arrays ---
echo "=== Test 1: Vector Addition ===\n";

$count = 1024 * 1024; // 1M elements
$a = range(0, $count - 1);
$b = array_map(fn($x) => $x * 2.0, range(0, $count - 1));

echo "Creating buffers ({$count} floats each)... ";
$bufferA = $device->createBufferFromData($a, Metal\FLOAT);
$bufferB = $device->createBufferFromData($b, Metal\FLOAT);
$bufferResult = $device->createBuffer($count * 4); // 4 bytes per float
echo "OK\n";

$addFunction = $library->getFunction('add_arrays');
$pipeline = $device->createComputePipelineState($addFunction);

echo "Max threads per threadgroup: " . $pipeline->getMaxTotalThreadsPerThreadgroup() . "\n";
echo "Thread execution width: " . $pipeline->getThreadExecutionWidth() . "\n";

$queue = $device->createCommandQueue();
$cmdBuffer = $queue->createCommandBuffer();
$encoder = $cmdBuffer->createComputeCommandEncoder();

$encoder->setComputePipelineState($pipeline);
$encoder->setBuffer($bufferA, 0, 0);
$encoder->setBuffer($bufferB, 0, 1);
$encoder->setBuffer($bufferResult, 0, 2);

$threadgroupSize = min($pipeline->getMaxTotalThreadsPerThreadgroup(), $count);
$encoder->dispatchThreads([$count, 1, 1], [$threadgroupSize, 1, 1]);
$encoder->endEncoding();

echo "Running on GPU... ";
$start = hrtime(true);
$cmdBuffer->commit();
$cmdBuffer->waitUntilCompleted();
$elapsed = (hrtime(true) - $start) / 1e6;
echo "Done in " . number_format($elapsed, 2) . " ms\n";

// Verify results
$results = $bufferResult->getContents(Metal\FLOAT, 10); // First 10
echo "First 10 results: ";
foreach ($results as $i => $val) {
    $expected = $a[$i] + $b[$i];
    echo "{$val}" . ($val == $expected ? "✓" : "✗({$expected})") . " ";
}
echo "\n\n";

// --- Test 2: Scalar multiply ---
echo "=== Test 2: Scalar Multiply ===\n";

$mulFunction = $library->getFunction('multiply_scalar');
$mulPipeline = $device->createComputePipelineState($mulFunction);

$input = array_map('floatval', range(1, 10));
$bufInput = $device->createBufferFromData($input, Metal\FLOAT);
$bufOutput = $device->createBuffer(count($input) * 4);

// Pass scalar value as bytes (float = 4 bytes)
$scalar = 3.14;
$scalarBytes = pack('f', $scalar);

$cmdBuffer2 = $queue->createCommandBuffer();
$encoder2 = $cmdBuffer2->createComputeCommandEncoder();
$encoder2->setComputePipelineState($mulPipeline);
$encoder2->setBuffer($bufInput, 0, 0);
$encoder2->setBuffer($bufOutput, 0, 1);
$encoder2->setBytes($scalarBytes, 2);
$encoder2->dispatchThreads([count($input), 1, 1], [count($input), 1, 1]);
$encoder2->endEncoding();

$cmdBuffer2->commit();
$cmdBuffer2->waitUntilCompleted();

$output = $bufOutput->getContents(Metal\FLOAT, count($input));
echo "Input:    " . implode(', ', array_map(fn($v) => sprintf('%.1f', $v), $input)) . "\n";
echo "Scalar:   {$scalar}\n";
echo "Output:   " . implode(', ', array_map(fn($v) => sprintf('%.2f', $v), $output)) . "\n";
echo "Expected: " . implode(', ', array_map(fn($v) => sprintf('%.2f', $v * $scalar), $input)) . "\n\n";

// --- Test 3: Raw buffer I/O ---
echo "=== Test 3: Raw Buffer I/O ===\n";

$buf = $device->createBuffer(64);
$buf->writeRawContents(pack('f*', 1.0, 2.0, 3.0, 4.0));
$raw = $buf->getRawContents(16); // 4 floats * 4 bytes
$unpacked = unpack('f4val', $raw);
echo "Written:  1.0, 2.0, 3.0, 4.0\n";
echo "Read raw: " . implode(', ', $unpacked) . "\n\n";

echo "All tests completed!\n";
