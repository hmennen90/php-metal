--TEST--
Metal BinaryArchive — addComputePipelineFunctions and addRenderPipelineFunctions
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

$lib = $device->createLibraryWithSource("
#include <metal_stdlib>
using namespace metal;
kernel void k(device float* d [[buffer(0)]], uint i [[thread_position_in_grid]]) { d[i] = 1.0; }
vertex float4 vs(uint vid [[vertex_id]]) { return float4(0,0,0,1); }
fragment float4 fs() { return float4(1,0,0,1); }
");

$baDesc = new Metal\BinaryArchiveDescriptor();
$ba = $device->createBinaryArchive($baDesc);
var_dump($ba instanceof Metal\BinaryArchive);

// Add compute pipeline
$cpd = new Metal\ComputePipelineDescriptor();
$cpd->setComputeFunction($lib->getFunction('k'));
$ba->addComputePipelineFunctions($cpd);
echo "Compute added\n";

// Add render pipeline
$rpd = new Metal\RenderPipelineDescriptor();
$rpd->setVertexFunction($lib->getFunction('vs'));
$rpd->setFragmentFunction($lib->getFunction('fs'));
$rpd->getColorAttachment(0)->setPixelFormat(Metal\PixelFormatBGRA8Unorm);
$ba->addRenderPipelineFunctions($rpd);
echo "Render added\n";

// Serialize
$tmpFile = tempnam(sys_get_temp_dir(), 'metal_ba_') . '.metallib';
$ba->serializeToURL($tmpFile);
var_dump(file_exists($tmpFile));
unlink($tmpFile);

echo "OK\n";
?>
--EXPECT--
bool(true)
Compute added
Render added
bool(true)
OK
