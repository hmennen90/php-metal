--TEST--
Metal MeshRenderPipelineDescriptor — color attachment and advanced properties
--EXTENSIONS--
metal
--FILE--
<?php
$desc = new Metal\MeshRenderPipelineDescriptor();

// Color attachment access
$ca = $desc->getColorAttachment(0);
var_dump($ca instanceof Metal\ColorAttachmentDescriptor);
$ca->setPixelFormat(Metal\PixelFormatBGRA8Unorm);
$ca->setBlendingEnabled(true);
$ca->setRgbBlendOperation(Metal\BlendOperationAdd);
$ca->setAlphaBlendOperation(Metal\BlendOperationAdd);
$ca->setWriteMask(Metal\ColorWriteMaskAll);

// Advanced properties
$desc->setStencilAttachmentPixelFormat(Metal\PixelFormatStencil8);
$desc->setRasterSampleCount(1);
$desc->setAlphaToCoverageEnabled(false);

echo "OK\n";
?>
--EXPECT--
bool(true)
OK
