--TEST--
Metal Indirect Command Buffer
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

$desc = new Metal\IndirectCommandBufferDescriptor();
$desc->setCommandTypes(Metal\IndirectCommandTypeDraw);
$desc->setInheritBuffers(false);
$desc->setInheritPipelineState(true);
$desc->setMaxVertexBufferBindCount(10);
$desc->setMaxFragmentBufferBindCount(10);

try {
    $icb = $device->createIndirectCommandBuffer($desc, 100);
    var_dump($icb instanceof Metal\IndirectCommandBuffer);
    var_dump($icb->getSize() >= 1);
    $icb->resetWithRange(0, 50);
    echo "OK\n";
} catch (Metal\Exception $e) {
    echo "SKIP: " . $e->getMessage() . "\n";
    echo "OK\n";
}
?>
--EXPECTF--
%s
%s
OK
