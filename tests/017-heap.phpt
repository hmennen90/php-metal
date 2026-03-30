--TEST--
Metal Heap — sub-allocation
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

$desc = new Metal\HeapDescriptor();
$desc->setSize(1024 * 1024); // 1 MB
$desc->setStorageMode(Metal\StorageModeShared);

$heap = $device->createHeap($desc);
var_dump($heap instanceof Metal\Heap);
var_dump($heap->getCurrentAllocatedSize() > 0);

$buf = $heap->createBuffer(4096);
var_dump($buf instanceof Metal\Buffer);
var_dump($buf->getLength() === 4096);
var_dump($heap->getUsedSize() > 0);

$maxAvail = $heap->getMaxAvailableSize(256);
var_dump($maxAvail > 0);

// heapBufferSizeAndAlign
$sa = $device->heapBufferSizeAndAlign(4096);
var_dump(is_array($sa));
var_dump(isset($sa['size']) && $sa['size'] >= 4096);
var_dump(isset($sa['align']) && $sa['align'] > 0);

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
OK
