--TEST--
Metal Indirect Command Buffer
--EXTENSIONS--
metal
--SKIPIF--
<?php if (getenv('METAL_SKIP_ICB_TESTS')) die('skip: ICB tests disabled'); ?>
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

$desc = new Metal\IndirectCommandBufferDescriptor();
$desc->setCommandTypes(Metal\IndirectCommandTypeDraw);
$desc->setInheritBuffers(false);
$desc->setInheritPipelineState(true);
$desc->setMaxVertexBufferBindCount(10);
$desc->setMaxFragmentBufferBindCount(10);

$icb = $device->createIndirectCommandBuffer($desc, 100);
var_dump($icb instanceof Metal\IndirectCommandBuffer);
var_dump($icb->getSize() >= 1);
$icb->resetWithRange(0, 50);
echo "OK\n";
?>
--EXPECT--
bool(true)
bool(true)
OK
