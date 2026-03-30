--TEST--
Metal Texture — makeAliasable, isAliasable
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

// Create from heap to test aliasable
$heapDesc = new Metal\HeapDescriptor();
$heapDesc->setSize(4 * 1024 * 1024);
$heapDesc->setStorageMode(Metal\StorageModePrivate);
$heap = $device->createHeap($heapDesc);

$texDesc = Metal\TextureDescriptor::texture2DDescriptor(Metal\PixelFormatRGBA8Unorm, 64, 64, false);
$texDesc->setStorageMode(Metal\StorageModePrivate);
$texDesc->setUsage(Metal\TextureUsageShaderRead);

$tex = $heap->createTexture($texDesc);
var_dump($tex instanceof Metal\Texture);

// Before making aliasable
var_dump($tex->isAliasable() === false);

// Make aliasable (allows heap to reuse memory)
$tex->makeAliasable();
var_dump($tex->isAliasable() === true);

echo "OK\n";
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
OK
