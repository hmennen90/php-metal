--TEST--
Metal namespace constants exist
--EXTENSIONS--
metal
--FILE--
<?php
// Data types
var_dump(defined('Metal\\FLOAT'));
var_dump(defined('Metal\\INT32'));
var_dump(defined('Metal\\UINT32'));
var_dump(defined('Metal\\DOUBLE'));

// Storage modes
var_dump(defined('Metal\\StorageModeShared'));
var_dump(defined('Metal\\StorageModePrivate'));

// Pixel formats
var_dump(defined('Metal\\PixelFormatRGBA8Unorm'));
var_dump(defined('Metal\\PixelFormatBGRA8Unorm'));
var_dump(defined('Metal\\PixelFormatDepth32Float'));

// Primitive types
var_dump(defined('Metal\\PrimitiveTypeTriangle'));

// Function types
var_dump(defined('Metal\\FunctionTypeVertex'));
var_dump(defined('Metal\\FunctionTypeFragment'));
var_dump(defined('Metal\\FunctionTypeKernel'));

// GPU families
var_dump(defined('Metal\\GPUFamilyApple1'));
var_dump(defined('Metal\\GPUFamilyMetal3'));

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
OK
