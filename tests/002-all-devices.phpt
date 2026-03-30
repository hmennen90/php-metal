--TEST--
Metal\allDevices — enumerate GPUs
--EXTENSIONS--
metal
--FILE--
<?php
$devices = Metal\allDevices();
var_dump(is_array($devices));
var_dump(count($devices) >= 1);
var_dump($devices[0] instanceof Metal\Device);
echo $devices[0]->getName() . "\n";
echo "OK\n";
?>
--EXPECTF--
bool(true)
bool(true)
bool(true)
%s
OK
