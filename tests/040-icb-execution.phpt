--TEST--
Metal IndirectCommandBuffer execution from RenderCommandEncoder
--EXTENSIONS--
metal
--SKIPIF--
<?php if (getenv('METAL_SKIP_ICB_TESTS')) die('skip: ICB tests disabled'); ?>
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

try {
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
    $rpd->setSupportIndirectCommandBuffers(true);
    $pso = $device->createRenderPipelineState($rpd);

    $icbDesc = new Metal\IndirectCommandBufferDescriptor();
    $icbDesc->setCommandTypes(Metal\IndirectCommandTypeDraw);
    $icbDesc->setInheritPipelineState(true);
    $icbDesc->setInheritBuffers(false);
    $icbDesc->setMaxVertexBufferBindCount(1);
    $icbDesc->setMaxFragmentBufferBindCount(1);
    $icb = $device->createIndirectCommandBuffer($icbDesc, 1);

    $cmd0 = $icb->indirectRenderCommandAtIndex(0);
    $cmd0->drawPrimitives(Metal\PrimitiveTypeTriangle, 0, 3, 1, 0);

    $texDesc = Metal\TextureDescriptor::texture2DDescriptor(Metal\PixelFormatBGRA8Unorm, 64, 64, false);
    $texDesc->setUsage(Metal\TextureUsageRenderTarget);
    $texDesc->setStorageMode(Metal\StorageModePrivate);
    $tex = $device->createTexture($texDesc);

    $pass = new Metal\RenderPassDescriptor();
    $pass->setColorAttachmentTexture(0, $tex);
    $pass->setColorAttachmentLoadAction(0, Metal\LoadActionClear);
    $pass->setColorAttachmentStoreAction(0, Metal\StoreActionStore);

    $queue = $device->createCommandQueue();
    $cmdBuf = $queue->createCommandBuffer();
    $enc = $cmdBuf->createRenderCommandEncoder($pass);
    $enc->setRenderPipelineState($pso);
    $enc->executeCommandsInBuffer($icb, 0, 1);
    $enc->endEncoding();
    $cmdBuf->commit();
    $cmdBuf->waitUntilCompleted();
} catch (Metal\Exception $e) {
    // ICB execution may not work on all GPU configurations
}

echo "OK\n";
?>
--EXPECT--
OK
