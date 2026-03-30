--TEST--
Metal BlitEncoder enhanced — generateMipmaps, fillBuffer
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();
$queue = $device->createCommandQueue();

// fillBuffer
$buf = $device->createBuffer(256);
$cmd = $queue->createCommandBuffer();
$blit = $cmd->createBlitCommandEncoder();
$blit->fillBuffer($buf, 0, 256, 0xFF);
$blit->endEncoding();
$cmd->commit();
$cmd->waitUntilCompleted();

$raw = $buf->getRawContents(4);
var_dump(ord($raw[0]) === 255);
var_dump(ord($raw[3]) === 255);

// generateMipmaps
$desc = Metal\TextureDescriptor::texture2DDescriptor(Metal\PixelFormatRGBA8Unorm, 64, 64, true);
$desc->setUsage(Metal\TextureUsageShaderRead | Metal\TextureUsageShaderWrite);
$tex = $device->createTexture($desc);

// Write base level
$pixels = str_repeat("\xFF\x80\x40\xFF", 64 * 64);
$tex->replaceRegion([0, 0, 64, 64], 0, $pixels, 64 * 4);

$cmd2 = $queue->createCommandBuffer();
$blit2 = $cmd2->createBlitCommandEncoder();
$blit2->generateMipmaps($tex);
$blit2->endEncoding();
$cmd2->commit();
$cmd2->waitUntilCompleted();

var_dump($tex->getMipmapLevelCount() > 1);

echo "OK\n";
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
OK
