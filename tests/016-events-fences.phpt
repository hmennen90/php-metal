--TEST--
Metal Events and Fences
--EXTENSIONS--
metal
--FILE--
<?php
$device = Metal\createSystemDefaultDevice();

$event = $device->createEvent();
var_dump($event instanceof Metal\Event);

$sharedEvent = $device->createSharedEvent();
var_dump($sharedEvent instanceof Metal\SharedEvent);
var_dump($sharedEvent->getSignaledValue() === 0);

$sharedEvent->setSignaledValue(42);
var_dump($sharedEvent->getSignaledValue() === 42);

$fence = $device->createFence();
var_dump($fence instanceof Metal\Fence);

echo "OK\n";
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
OK
