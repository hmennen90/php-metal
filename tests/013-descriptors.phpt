--TEST--
Metal descriptors — RenderPipeline, RenderPass, DepthStencil, Vertex, Sampler
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

// TextureDescriptor
$td = new Metal\TextureDescriptor();
$td->setPixelFormat(Metal\PixelFormatRGBA8Unorm);
$td->setWidth(128);
$td->setHeight(128);
$td->setTextureType(Metal\TextureType2D);
$td->setUsage(Metal\TextureUsageRenderTarget);
$td->setStorageMode(Metal\StorageModePrivate);
echo "TextureDescriptor OK\n";

// RenderPipelineDescriptor
$rpd = new Metal\RenderPipelineDescriptor();
$ca = $rpd->getColorAttachment(0);
$ca->setPixelFormat(Metal\PixelFormatBGRA8Unorm);
$ca->setBlendingEnabled(true);
$ca->setSourceRGBBlendFactor(Metal\BlendFactorSourceAlpha);
$ca->setDestinationRGBBlendFactor(Metal\BlendFactorOneMinusSourceAlpha);
$rpd->setDepthAttachmentPixelFormat(Metal\PixelFormatDepth32Float);
echo "RenderPipelineDescriptor OK\n";

// RenderPassDescriptor
$pass = new Metal\RenderPassDescriptor();
$pass->setColorAttachmentLoadAction(0, Metal\LoadActionClear);
$pass->setColorAttachmentStoreAction(0, Metal\StoreActionStore);
$pass->setColorAttachmentClearColor(0, 0.1, 0.2, 0.3, 1.0);
$pass->setDepthAttachmentLoadAction(Metal\LoadActionClear);
$pass->setDepthAttachmentStoreAction(Metal\StoreActionDontCare);
$pass->setDepthAttachmentClearDepth(1.0);
echo "RenderPassDescriptor OK\n";

// DepthStencilDescriptor
$dsd = new Metal\DepthStencilDescriptor();
$dsd->setDepthCompareFunction(Metal\CompareFunctionLess);
$dsd->setDepthWriteEnabled(true);
$dss = $device->createDepthStencilState($dsd);
var_dump($dss instanceof Metal\DepthStencilState);

// VertexDescriptor
$vd = new Metal\VertexDescriptor();
$vd->setAttribute(0, Metal\VertexFormatFloat3, 0, 0);  // position
$vd->setAttribute(1, Metal\VertexFormatFloat3, 12, 0);  // normal
$vd->setLayout(0, 24, Metal\VertexStepFunctionPerVertex);
echo "VertexDescriptor OK\n";

// SamplerDescriptor
$sd = new Metal\SamplerDescriptor();
$sd->setMinFilter(Metal\SamplerMinMagFilterLinear);
$sd->setMagFilter(Metal\SamplerMinMagFilterLinear);
$sd->setSAddressMode(Metal\SamplerAddressModeRepeat);
$sd->setTAddressMode(Metal\SamplerAddressModeRepeat);
$sampler = $device->createSamplerState($sd);
var_dump($sampler instanceof Metal\SamplerState);

echo "All descriptors OK\n";
?>
--EXPECT--
TextureDescriptor OK
RenderPipelineDescriptor OK
RenderPassDescriptor OK
bool(true)
VertexDescriptor OK
bool(true)
All descriptors OK
