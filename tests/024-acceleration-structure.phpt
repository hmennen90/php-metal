--TEST--
Metal Acceleration Structure descriptors
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

// Triangle geometry descriptor
$geom = new Metal\AccelerationStructureTriangleGeometryDescriptor();
var_dump($geom instanceof Metal\AccelerationStructureTriangleGeometryDescriptor);

$vertices = $device->createBufferFromData([
    0.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    0.5, 1.0, 0.0,
]);
$geom->setVertexBuffer($vertices);
$geom->setVertexStride(12); // 3 floats * 4 bytes
$geom->setTriangleCount(1);

// Primitive acceleration structure descriptor
$primDesc = new Metal\PrimitiveAccelerationStructureDescriptor();
$primDesc->setGeometryDescriptors([$geom]);
var_dump($primDesc instanceof Metal\PrimitiveAccelerationStructureDescriptor);

// Get sizes
$sizes = $device->getAccelerationStructureSizes($primDesc);
var_dump(is_array($sizes));
var_dump(isset($sizes['accelerationStructureSize']));
var_dump($sizes['accelerationStructureSize'] > 0);
var_dump(isset($sizes['buildScratchBufferSize']));

// Create acceleration structure
$as = $device->createAccelerationStructureWithSize($sizes['accelerationStructureSize']);
var_dump($as instanceof Metal\AccelerationStructure);
var_dump($as->getSize() > 0);

// Instance acceleration structure descriptor
$instDesc = new Metal\InstanceAccelerationStructureDescriptor();
$instDesc->setInstanceCount(1);
var_dump($instDesc instanceof Metal\InstanceAccelerationStructureDescriptor);

// BoundingBox geometry descriptor
$bbox = new Metal\AccelerationStructureBoundingBoxGeometryDescriptor();
var_dump($bbox instanceof Metal\AccelerationStructureBoundingBoxGeometryDescriptor);

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
OK
