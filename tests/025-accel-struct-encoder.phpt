--TEST--
Metal AccelerationStructureCommandEncoder — build acceleration structure
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

// Create triangle geometry
$vertices = $device->createBufferFromData([
    0.0, 0.0, 0.0,  1.0, 0.0, 0.0,  0.5, 1.0, 0.0,
]);
$geom = new Metal\AccelerationStructureTriangleGeometryDescriptor();
$geom->setVertexBuffer($vertices);
$geom->setVertexStride(12);
$geom->setTriangleCount(1);

$primDesc = new Metal\PrimitiveAccelerationStructureDescriptor();
$primDesc->setGeometryDescriptors([$geom]);

// Get sizes and create AS + scratch buffer
$sizes = $device->getAccelerationStructureSizes($primDesc);
$as = $device->createAccelerationStructureWithSize($sizes['accelerationStructureSize']);
$scratch = $device->createBuffer($sizes['buildScratchBufferSize']);

// Build using AccelerationStructureCommandEncoder
$queue = $device->createCommandQueue();
$cmd = $queue->createCommandBuffer();
$enc = $cmd->createAccelerationStructureCommandEncoder();
var_dump($enc instanceof Metal\AccelerationStructureCommandEncoder);

$enc->buildAccelerationStructure($as, $primDesc, $scratch);
$enc->endEncoding();
$cmd->commit();
$cmd->waitUntilCompleted();

var_dump($as->getSize() > 0);
echo "OK\n";
?>
--EXPECT--
bool(true)
bool(true)
OK
