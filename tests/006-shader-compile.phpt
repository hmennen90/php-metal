--TEST--
Metal\Library — shader compilation
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

$source = <<<'MSL'
#include <metal_stdlib>
using namespace metal;

kernel void test_kernel(
    device float* data [[buffer(0)]],
    uint id [[thread_position_in_grid]])
{
    data[id] = data[id] * 2.0;
}

vertex float4 test_vertex(uint vid [[vertex_id]]) {
    return float4(0, 0, 0, 1);
}
MSL;

$library = $device->createLibraryWithSource($source);
var_dump($library instanceof Metal\Library);

$names = $library->getFunctionNames();
var_dump(count($names) === 2);
var_dump(in_array('test_kernel', $names));
var_dump(in_array('test_vertex', $names));

$fn = $library->getFunction('test_kernel');
var_dump($fn instanceof Metal\MetalFunction);
var_dump($fn->getName() === 'test_kernel');
var_dump($fn->getFunctionType() === Metal\FunctionTypeKernel);

$vfn = $library->getFunction('test_vertex');
var_dump($vfn->getFunctionType() === Metal\FunctionTypeVertex);

echo "OK\n";
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
OK
