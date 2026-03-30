--TEST--
Metal Buffer-backed texture
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

// Create a buffer big enough for a 4x4 RGBA8 texture
$bytesPerRow = 4 * 4; // 4 pixels * 4 bytes
$buf = $device->createBuffer($bytesPerRow * 4); // 4 rows

// Write pixel data to buffer
$pixels = str_repeat("\xFF\x00\xFF\xFF", 16); // 16 magenta pixels
$buf->writeRawContents($pixels);

// Create texture backed by buffer
$texDesc = Metal\TextureDescriptor::texture2DDescriptor(Metal\PixelFormatRGBA8Unorm, 4, 4, false);
$texDesc->setUsage(Metal\TextureUsageShaderRead);

$tex = $buf->newTextureFromBuffer($texDesc, 0, $bytesPerRow);
var_dump($tex instanceof Metal\Texture);
var_dump($tex->getWidth() === 4);
var_dump($tex->getHeight() === 4);

echo "OK\n";
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
OK
