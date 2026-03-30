--TEST--
Metal RenderCommandEncoder — comprehensive method coverage
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

$lib = $device->createLibraryWithSource("
#include <metal_stdlib>
using namespace metal;
vertex float4 vs(uint vid [[vertex_id]]) { return float4(0,0,0,1); }
fragment float4 fs() { return float4(1,0,0,1); }
");

$rpd = new Metal\RenderPipelineDescriptor();
$rpd->setVertexFunction($lib->getFunction('vs'));
$rpd->setFragmentFunction($lib->getFunction('fs'));
$rpd->getColorAttachment(0)->setPixelFormat(Metal\PixelFormatBGRA8Unorm);
$pso = $device->createRenderPipelineState($rpd);

$texDesc = Metal\TextureDescriptor::texture2DDescriptor(Metal\PixelFormatBGRA8Unorm, 64, 64, false);
$texDesc->setUsage(Metal\TextureUsageRenderTarget);
$texDesc->setStorageMode(Metal\StorageModePrivate);
$tex = $device->createTexture($texDesc);

$pass = new Metal\RenderPassDescriptor();
$pass->setColorAttachmentTexture(0, $tex);
$pass->setColorAttachmentLoadAction(0, Metal\LoadActionClear);
$pass->setColorAttachmentStoreAction(0, Metal\StoreActionStore);
$pass->setColorAttachmentClearColor(0, 0.0, 0.0, 0.0, 1.0);

$queue = $device->createCommandQueue();
$cmd = $queue->createCommandBuffer();
$enc = $cmd->createRenderCommandEncoder($pass);

$enc->setRenderPipelineState($pso);
$enc->setViewport(0, 0, 64, 64, 0.0, 1.0);
$enc->setScissorRect(0, 0, 64, 64);
$enc->setCullMode(Metal\CullModeBack);
$enc->setFrontFacingWinding(Metal\WindingCounterClockwise);
$enc->setTriangleFillMode(Metal\TriangleFillModeFill);
$enc->setBlendColor(1.0, 1.0, 1.0, 1.0);
$enc->setStencilReferenceValue(0);
$enc->setDepthBias(0.0, 0.0, 0.0);
$enc->setVertexAmplificationCount(1);

// Vertex/fragment bytes
$enc->setVertexBytes(pack('f4', 1.0, 0.0, 0.0, 1.0), 1);
$enc->setFragmentBytes(pack('f4', 1.0, 0.0, 0.0, 1.0), 0);

// Debug groups
$enc->pushDebugGroup('test-group');
$enc->popDebugGroup();

$enc->drawPrimitives(Metal\PrimitiveTypeTriangle, 0, 3);
$enc->drawPrimitivesInstanced(Metal\PrimitiveTypeTriangle, 0, 3, 1);

$enc->endEncoding();
$cmd->commit();
$cmd->waitUntilCompleted();

echo "OK\n";
?>
--EXPECT--
OK
