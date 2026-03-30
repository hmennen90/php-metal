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
$desc->setInheritPipelineState(false);
$desc->setMaxVertexBufferBindCount(10);
$desc->setMaxFragmentBufferBindCount(10);

$icb = $device->createIndirectCommandBuffer($desc, 100);
var_dump($icb instanceof Metal\IndirectCommandBuffer);
var_dump($icb->getSize() === 100);

$icb->resetWithRange(0, 50);
echo "OK\n";
?>
--EXPECT--
bool(true)
bool(true)
OK
