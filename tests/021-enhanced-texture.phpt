--TEST--
Metal Texture enhanced methods
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

$desc = Metal\TextureDescriptor::texture2DDescriptor(Metal\PixelFormatRGBA8Unorm, 64, 64, true);
$desc->setUsage(Metal\TextureUsageShaderRead | Metal\TextureUsageShaderWrite);
$tex = $device->createTexture($desc);

var_dump($tex->getUsage() > 0);
var_dump($tex->getStorageMode() >= 0);
var_dump($tex->getMipmapLevelCount() > 1); // mipmapped = true
var_dump($tex->getArrayLength() === 1);
var_dump($tex->getSampleCount() === 1);
var_dump($tex->isFramebufferOnly() === false);

// newTextureView
$view = $tex->newTextureView(Metal\PixelFormatRGBA8Unorm);
var_dump($view instanceof Metal\Texture);
var_dump($view->getWidth() === 64);

echo "OK\n";
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
OK
