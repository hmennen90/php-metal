--TEST--
Metal ColorAttachment blend ops + RenderPass stencil attachment
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

// Blend operations on color attachment
$rpd = new Metal\RenderPipelineDescriptor();
$ca = $rpd->getColorAttachment(0);
$ca->setPixelFormat(Metal\PixelFormatBGRA8Unorm);
$ca->setBlendingEnabled(true);
$ca->setRgbBlendOperation(Metal\BlendOperationAdd);
$ca->setAlphaBlendOperation(Metal\BlendOperationAdd);
$ca->setSourceRGBBlendFactor(Metal\BlendFactorSourceAlpha);
$ca->setDestinationRGBBlendFactor(Metal\BlendFactorOneMinusSourceAlpha);
$ca->setWriteMask(Metal\ColorWriteMaskAll);
echo "Blend OK\n";

// Stencil attachment on render pass
$pass = new Metal\RenderPassDescriptor();

$texDesc = Metal\TextureDescriptor::texture2DDescriptor(Metal\PixelFormatDepth32Float_Stencil8, 64, 64, false);
$texDesc->setUsage(Metal\TextureUsageRenderTarget);
$texDesc->setStorageMode(Metal\StorageModePrivate);
$depthStencilTex = $device->createTexture($texDesc);

$pass->setStencilAttachmentTexture($depthStencilTex);
$pass->setStencilAttachmentLoadAction(Metal\LoadActionClear);
$pass->setStencilAttachmentStoreAction(Metal\StoreActionDontCare);
$pass->setStencilAttachmentClearStencil(0);
echo "Stencil pass OK\n";

// MSAA resolve texture
$colorDesc = Metal\TextureDescriptor::texture2DDescriptor(Metal\PixelFormatBGRA8Unorm, 64, 64, false);
$colorDesc->setStorageMode(Metal\StorageModePrivate);
$colorDesc->setUsage(Metal\TextureUsageRenderTarget);
$resolveTex = $device->createTexture($colorDesc);
$pass->setColorAttachmentResolveTexture(0, $resolveTex);
$pass->setDepthAttachmentResolveTexture($depthStencilTex);
echo "Resolve OK\n";
?>
--EXPECT--
Blend OK
Stencil pass OK
Resolve OK
