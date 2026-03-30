--TEST--
Metal StencilDescriptor and stencil on DepthStencilDescriptor
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

$stencil = new Metal\StencilDescriptor();
var_dump($stencil instanceof Metal\StencilDescriptor);

$stencil->setStencilCompareFunction(Metal\CompareFunctionAlways);
$stencil->setStencilFailureOperation(Metal\StencilOperationKeep);
$stencil->setDepthFailureOperation(Metal\StencilOperationKeep);
$stencil->setDepthStencilPassOperation(Metal\StencilOperationReplace);
$stencil->setReadMask(0xFF);
$stencil->setWriteMask(0xFF);

$dsd = new Metal\DepthStencilDescriptor();
$dsd->setDepthCompareFunction(Metal\CompareFunctionLess);
$dsd->setDepthWriteEnabled(true);
$dsd->setFrontFaceStencil($stencil);
$dsd->setBackFaceStencil($stencil);

$state = $device->createDepthStencilState($dsd);
var_dump($state instanceof Metal\DepthStencilState);

// Verify stencil constants
var_dump(Metal\StencilOperationKeep === 0);
var_dump(Metal\StencilOperationReplace === 2);
var_dump(Metal\StencilOperationInvert === 5);

echo "OK\n";
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
OK
