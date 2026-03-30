--TEST--
Metal ArgumentEncoder — comprehensive coverage
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

$lib = $device->createLibraryWithSource("
#include <metal_stdlib>
using namespace metal;
struct Args { device float* data; texture2d<float> tex; sampler smp; };
kernel void k(device Args& args [[buffer(0)]], uint i [[thread_position_in_grid]]) { args.data[i] = 1.0; }
");

$fn = $lib->getFunction('k');
$enc = $fn->createArgumentEncoder(0);
var_dump($enc instanceof Metal\ArgumentEncoder);
var_dump($enc->getEncodedLength() > 0);

// Create argument buffer
$argBuf = $device->createBuffer($enc->getEncodedLength());
$enc->setArgumentBuffer($argBuf, 0);

// Bind resources
$dataBuf = $device->createBuffer(256);
$enc->setBuffer($dataBuf, 0, 0);

$texDesc = Metal\TextureDescriptor::texture2DDescriptor(Metal\PixelFormatRGBA8Unorm, 4, 4, false);
$tex = $device->createTexture($texDesc);
$enc->setTexture($tex, 1);

$smpDesc = new Metal\SamplerDescriptor();
$smp = $device->createSamplerState($smpDesc);
$enc->setSamplerState($smp, 2);

echo "OK\n";
?>
--EXPECT--
bool(true)
bool(true)
OK
