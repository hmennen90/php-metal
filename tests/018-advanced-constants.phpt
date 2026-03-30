--TEST--
Metal advanced constants exist
--EXTENSIONS--
metal
--FILE--
<?php
// Heap types
var_dump(defined('Metal\\HeapTypeAutomatic'));
var_dump(defined('Metal\\HeapTypePlacement'));

// Hazard tracking
var_dump(defined('Metal\\HazardTrackingModeDefault'));
var_dump(defined('Metal\\HazardTrackingModeUntracked'));

// Resource usage
var_dump(defined('Metal\\ResourceUsageRead'));
var_dump(defined('Metal\\ResourceUsageWrite'));

// Barrier scope
var_dump(defined('Metal\\BarrierScopeBuffers'));
var_dump(defined('Metal\\BarrierScopeTextures'));

// Render stages
var_dump(defined('Metal\\RenderStageVertex'));
var_dump(defined('Metal\\RenderStageFragment'));

// Dispatch types
var_dump(defined('Metal\\DispatchTypeSerial'));
var_dump(defined('Metal\\DispatchTypeConcurrent'));

// Indirect command types
var_dump(defined('Metal\\IndirectCommandTypeDraw'));

// Acceleration structure usage
var_dump(defined('Metal\\AccelerationStructureUsageNone'));

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
bool(true)
bool(true)
bool(true)
OK
