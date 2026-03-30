--TEST--
Metal RenderPipelineDescriptor — tessellation and advanced properties
--EXTENSIONS--
metal
--FILE--
<?php
$rpd = new Metal\RenderPipelineDescriptor();

// Tessellation
$rpd->setTessellationPartitionMode(Metal\TessellationPartitionModeInteger);
$rpd->setMaxTessellationFactor(16);
$rpd->setTessellationFactorScaleEnabled(false);
$rpd->setTessellationFactorFormat(Metal\TessellationFactorFormatHalf);
$rpd->setTessellationControlPointIndexType(Metal\TessellationControlPointIndexTypeNone);
$rpd->setTessellationFactorStepFunction(Metal\TessellationFactorStepFunctionPerPatch);
$rpd->setTessellationOutputWindingOrder(Metal\WindingClockwise);

// Advanced
$rpd->setStencilAttachmentPixelFormat(Metal\PixelFormatStencil8);
$rpd->setInputPrimitiveTopology(Metal\PrimitiveTopologyClassTriangle);
$rpd->setSupportIndirectCommandBuffers(true);
$rpd->setRasterSampleCount(1);
$rpd->setAlphaToCoverageEnabled(false);
$rpd->setAlphaToOneEnabled(false);
$rpd->setRasterizationEnabled(true);

echo "OK\n";
?>
--EXPECT--
OK
