--TEST--
Metal tessellation, blend, stencil constants
--EXTENSIONS--
metal
--FILE--
<?php
// Tessellation
var_dump(defined('Metal\\TessellationPartitionModePow2'));
var_dump(defined('Metal\\TessellationPartitionModeInteger'));
var_dump(defined('Metal\\TessellationPartitionModeFractionalOdd'));
var_dump(defined('Metal\\TessellationFactorFormatHalf'));
var_dump(defined('Metal\\TessellationControlPointIndexTypeNone'));
var_dump(defined('Metal\\TessellationFactorStepFunctionConstant'));

// Blend operations
var_dump(defined('Metal\\BlendOperationAdd'));
var_dump(defined('Metal\\BlendOperationSubtract'));
var_dump(defined('Metal\\BlendOperationMin'));

// Color write mask
var_dump(defined('Metal\\ColorWriteMaskAll'));
var_dump(defined('Metal\\ColorWriteMaskNone'));
var_dump(defined('Metal\\ColorWriteMaskRed'));

// Stencil pixel formats
var_dump(defined('Metal\\PixelFormatStencil8'));
var_dump(defined('Metal\\PixelFormatDepth32Float_Stencil8'));

// MSAA store actions
var_dump(defined('Metal\\StoreActionMultisampleResolve'));

// Topology class
var_dump(defined('Metal\\PrimitiveTopologyClassTriangle'));

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
bool(true)
bool(true)
OK
