--TEST--
Metal\Texture — create and read/write
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

// Create texture descriptor
$desc = Metal\TextureDescriptor::texture2DDescriptor(
    Metal\PixelFormatRGBA8Unorm,
    4, 4,
    false
);
var_dump($desc instanceof Metal\TextureDescriptor);
$desc->setUsage(Metal\TextureUsageShaderRead | Metal\TextureUsageShaderWrite);

$texture = $device->createTexture($desc);
var_dump($texture instanceof Metal\Texture);
var_dump($texture->getWidth() === 4);
var_dump($texture->getHeight() === 4);
var_dump($texture->getTextureType() === Metal\TextureType2D);
var_dump($texture->getPixelFormat() === Metal\PixelFormatRGBA8Unorm);

// Write pixel data (4x4 RGBA = 64 bytes)
$pixels = str_repeat("\xFF\x00\x00\xFF", 16); // 16 red pixels
$texture->replaceRegion([0, 0, 4, 4], 0, $pixels, 4 * 4);

// Read back
$readback = $texture->getBytes([0, 0, 4, 4], 0, 4 * 4);
var_dump(strlen($readback) === 64);
var_dump(ord($readback[0]) === 255); // R
var_dump(ord($readback[1]) === 0);   // G
var_dump(ord($readback[2]) === 0);   // B
var_dump(ord($readback[3]) === 255); // A

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
bool(true)
bool(true)
bool(true)
OK
