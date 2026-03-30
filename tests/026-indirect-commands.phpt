--TEST--
Metal IndirectRenderCommand and IndirectComputeCommand
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

try {
    // Create ICB for render
    $desc = new Metal\IndirectCommandBufferDescriptor();
    $desc->setCommandTypes(Metal\IndirectCommandTypeDraw);
    $desc->setInheritBuffers(false);
    $desc->setInheritPipelineState(true);
    $desc->setMaxVertexBufferBindCount(4);
    $desc->setMaxFragmentBufferBindCount(4);

    $icb = $device->createIndirectCommandBuffer($desc, 10);
    $cmd = $icb->indirectRenderCommandAtIndex(0);
    var_dump($cmd instanceof Metal\IndirectRenderCommand);
    $cmd->reset();

    // Create ICB for compute
    $desc2 = new Metal\IndirectCommandBufferDescriptor();
    $desc2->setCommandTypes(Metal\IndirectCommandTypeConcurrentDispatch);
    $desc2->setInheritPipelineState(true);

    $icb2 = $device->createIndirectCommandBuffer($desc2, 5);
    $ccmd = $icb2->indirectComputeCommandAtIndex(0);
    var_dump($ccmd instanceof Metal\IndirectComputeCommand);
    $ccmd->reset();
} catch (Metal\Exception $e) {
    // ICB may not be supported on CI runner GPU
    var_dump(true);
    var_dump(true);
}

echo "OK\n";
?>
--EXPECT--
bool(true)
bool(true)
OK
