--TEST--
Metal MeshRenderPipelineDescriptor
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

$desc = new Metal\MeshRenderPipelineDescriptor();
var_dump($desc instanceof Metal\MeshRenderPipelineDescriptor);

$desc->setMaxTotalThreadsPerObjectThreadgroup(64);
$desc->setMaxTotalThreadsPerMeshThreadgroup(128);
$desc->setMaxTotalThreadgroupsPerMeshGrid(1);
$desc->setPayloadMemoryLength(1024);
$desc->setDepthAttachmentPixelFormat(Metal\PixelFormatDepth32Float);

echo "OK\n";
?>
--EXPECT--
bool(true)
OK
