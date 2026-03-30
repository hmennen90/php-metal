# php-metal

PHP extension for Apple's [Metal GPU API](https://developer.apple.com/metal/). Run compute shaders, render pipelines, and GPU-accelerated workloads directly from PHP.

**macOS only** (Apple Silicon & Intel Macs with Metal support).

## Requirements

- macOS 14 (Sonoma) or later
- PHP 8.2 - 8.5
- Xcode Command Line Tools (`xcode-select --install`)

## Installation

```bash
git clone https://github.com/hmennen90/php-metal.git
cd php-metal
phpize
./configure --enable-metal
make
make test
sudo make install
```

Then add to your `php.ini`:

```ini
extension=metal
```

Or load it per-invocation:

```bash
php -d extension=metal your_script.php
```

## Quick Start

### GPU Compute: Add Two Arrays

```php
<?php
$device = Metal\createSystemDefaultDevice();
echo "GPU: " . $device->getName() . "\n";

// Compile a Metal shader
$library = $device->createLibraryWithSource(<<<'MSL'
    #include <metal_stdlib>
    using namespace metal;
    kernel void add_arrays(
        device const float* a [[buffer(0)]],
        device const float* b [[buffer(1)]],
        device float* result   [[buffer(2)]],
        uint index [[thread_position_in_grid]])
    {
        result[index] = a[index] + b[index];
    }
MSL);

// Create pipeline
$pipeline = $device->createComputePipelineState(
    $library->getFunction('add_arrays')
);

// Create buffers with data
$bufA = $device->createBufferFromData([1.0, 2.0, 3.0, 4.0]);
$bufB = $device->createBufferFromData([10.0, 20.0, 30.0, 40.0]);
$bufResult = $device->createBuffer(4 * 4); // 4 floats

// Encode and dispatch
$queue = $device->createCommandQueue();
$cmd = $queue->createCommandBuffer();
$enc = $cmd->createComputeCommandEncoder();
$enc->setComputePipelineState($pipeline);
$enc->setBuffer($bufA, 0, 0);
$enc->setBuffer($bufB, 0, 1);
$enc->setBuffer($bufResult, 0, 2);
$enc->dispatchThreads([4, 1, 1], [4, 1, 1]);
$enc->endEncoding();

$cmd->commit();
$cmd->waitUntilCompleted();

// Read results back
$results = $bufResult->getContents(Metal\FLOAT, 4);
print_r($results); // [11.0, 22.0, 33.0, 44.0]
```

## API Reference

### Functions

| Function | Description |
|----------|-------------|
| `Metal\createSystemDefaultDevice(): Device` | Get the default Metal GPU device |
| `Metal\allDevices(): array` | List all Metal-capable GPUs |

### Metal\Device

| Method | Description |
|--------|-------------|
| `getName(): string` | GPU name (e.g. "Apple M2") |
| `isLowPower(): bool` | Whether this is an integrated/low-power GPU |
| `isRemovable(): bool` | Whether this is an external GPU |
| `getRegistryID(): int` | Unique GPU registry identifier |
| `getMaxBufferLength(): int` | Maximum buffer size in bytes |
| `getMaxThreadgroupMemoryLength(): int` | Maximum threadgroup memory |
| `getMaxThreadsPerThreadgroup(): array` | Max [width, height, depth] |
| `supportsFamily(int $family): bool` | Check GPU family support |
| `createBuffer(int $length, int $options = 0): Buffer` | Create empty buffer |
| `createBufferFromData(array $data, int $type = FLOAT, int $options = 0): Buffer` | Create buffer from PHP array |
| `createCommandQueue(): CommandQueue` | Create a command queue |
| `createLibraryWithSource(string $source, ?array $options = null): Library` | Compile MSL shader source |
| `createLibraryWithFile(string $path): Library` | Load precompiled .metallib |
| `createDefaultLibrary(): Library` | Load default .metallib from app bundle |
| `createComputePipelineState(MetalFunction $fn): ComputePipelineState` | Create compute pipeline |
| `createRenderPipelineState(RenderPipelineDescriptor $desc): RenderPipelineState` | Create render pipeline |
| `createTexture(TextureDescriptor $desc): Texture` | Create a texture |
| `createDepthStencilState(DepthStencilDescriptor $desc): DepthStencilState` | Create depth/stencil state |
| `createSamplerState(SamplerDescriptor $desc): SamplerState` | Create a texture sampler |

### Metal\Buffer

| Method | Description |
|--------|-------------|
| `getLength(): int` | Buffer size in bytes |
| `getContents(int $type = FLOAT, int $count = -1, int $offset = 0): array` | Read typed data to PHP array |
| `writeContents(array $data, int $type = FLOAT, int $offset = 0): void` | Write PHP array to buffer |
| `getRawContents(int $length = -1, int $offset = 0): string` | Read raw bytes |
| `writeRawContents(string $data, int $offset = 0): void` | Write raw bytes |

### Metal\CommandQueue / CommandBuffer

| Method | Description |
|--------|-------------|
| `CommandQueue::createCommandBuffer(): CommandBuffer` | Create a command buffer |
| `CommandBuffer::createComputeCommandEncoder(): ComputeCommandEncoder` | Start compute encoding |
| `CommandBuffer::createRenderCommandEncoder(RenderPassDescriptor $d): RenderCommandEncoder` | Start render encoding |
| `CommandBuffer::createBlitCommandEncoder(): BlitCommandEncoder` | Start blit encoding |
| `CommandBuffer::commit(): void` | Submit to GPU |
| `CommandBuffer::waitUntilCompleted(): void` | Block until GPU finishes |
| `CommandBuffer::waitUntilScheduled(): void` | Block until scheduled |
| `CommandBuffer::getStatus(): int` | Current status |
| `CommandBuffer::presentDrawable(Drawable $d): void` | Present a drawable |

### Metal\ComputeCommandEncoder

| Method | Description |
|--------|-------------|
| `setComputePipelineState(ComputePipelineState $s): void` | Set the compute pipeline |
| `setBuffer(Buffer $buf, int $offset, int $index): void` | Bind a buffer |
| `setBytes(string $data, int $index): void` | Set inline bytes (small data) |
| `setTexture(Texture $tex, int $index): void` | Bind a texture |
| `setSamplerState(SamplerState $s, int $index): void` | Bind a sampler |
| `setThreadgroupMemoryLength(int $len, int $index): void` | Set threadgroup memory |
| `dispatchThreads(array $grid, array $threadgroup): void` | Dispatch by total threads |
| `dispatchThreadgroups(array $groups, array $threads): void` | Dispatch by threadgroup count |
| `endEncoding(): void` | Finish encoding |

### Metal\RenderCommandEncoder

| Method | Description |
|--------|-------------|
| `setRenderPipelineState(RenderPipelineState $s): void` | Set render pipeline |
| `setVertexBuffer(Buffer $buf, int $offset, int $index): void` | Bind vertex buffer |
| `setVertexBytes(string $data, int $index): void` | Set inline vertex bytes |
| `setFragmentBuffer(Buffer $buf, int $offset, int $index): void` | Bind fragment buffer |
| `setFragmentBytes(string $data, int $index): void` | Set inline fragment bytes |
| `setFragmentTexture(Texture $tex, int $index): void` | Bind fragment texture |
| `setFragmentSamplerState(SamplerState $s, int $index): void` | Bind fragment sampler |
| `setDepthStencilState(DepthStencilState $s): void` | Set depth/stencil state |
| `setViewport(float $x, $y, $w, $h, $znear, $zfar): void` | Set viewport |
| `setScissorRect(int $x, $y, $w, $h): void` | Set scissor rectangle |
| `setCullMode(int $mode): void` | Set face culling |
| `setFrontFacingWinding(int $winding): void` | Set front-face winding |
| `setTriangleFillMode(int $mode): void` | Wireframe or fill |
| `drawPrimitives(int $type, int $start, int $count): void` | Draw primitives |
| `drawIndexedPrimitives(int $type, int $count, int $indexType, Buffer $buf, int $offset): void` | Indexed draw |
| `endEncoding(): void` | Finish encoding |

### Metal\BlitCommandEncoder

| Method | Description |
|--------|-------------|
| `copyFromBuffer(Buffer $src, int $srcOff, Buffer $dst, int $dstOff, int $size): void` | Copy buffer data |
| `synchronizeResource(Buffer $buf): void` | Synchronize managed resource |
| `endEncoding(): void` | Finish encoding |

### Metal\Library / MetalFunction / Pipeline

| Method | Description |
|--------|-------------|
| `Library::getFunction(string $name): MetalFunction` | Get a shader function |
| `Library::getFunctionNames(): array` | List all function names |
| `MetalFunction::getName(): string` | Function name |
| `MetalFunction::getFunctionType(): int` | Vertex, fragment, or kernel |
| `ComputePipelineState::getMaxTotalThreadsPerThreadgroup(): int` | Max threads |
| `ComputePipelineState::getThreadExecutionWidth(): int` | SIMD width |

### Metal\Texture

| Method | Description |
|--------|-------------|
| `getWidth(): int` | Texture width |
| `getHeight(): int` | Texture height |
| `getDepth(): int` | Texture depth |
| `getPixelFormat(): int` | Pixel format constant |
| `getTextureType(): int` | Texture type constant |
| `replaceRegion(array $region, int $mip, string $data, int $bytesPerRow): void` | Upload pixel data |
| `getBytes(array $region, int $mip, int $bytesPerRow): string` | Download pixel data |

### Descriptor Classes

**TextureDescriptor** — `new TextureDescriptor()` or `TextureDescriptor::texture2DDescriptor(format, w, h, mip)`
Methods: `setPixelFormat`, `setWidth`, `setHeight`, `setTextureType`, `setUsage`, `setStorageMode`

**RenderPipelineDescriptor** — `new RenderPipelineDescriptor()`
Methods: `setVertexFunction`, `setFragmentFunction`, `getColorAttachment($i)`, `setDepthAttachmentPixelFormat`, `setVertexDescriptor`

**RenderPassDescriptor** — `new RenderPassDescriptor()`
Methods: `setColorAttachmentTexture`, `setColorAttachmentLoadAction`, `setColorAttachmentStoreAction`, `setColorAttachmentClearColor`, `setDepthAttachmentTexture`, `setDepthAttachmentLoadAction`, `setDepthAttachmentStoreAction`, `setDepthAttachmentClearDepth`

**ColorAttachmentDescriptor** (from `RenderPipelineDescriptor::getColorAttachment()`)
Methods: `setPixelFormat`, `setBlendingEnabled`, `setSourceRGBBlendFactor`, `setDestinationRGBBlendFactor`, `setSourceAlphaBlendFactor`, `setDestinationAlphaBlendFactor`

**DepthStencilDescriptor** — `new DepthStencilDescriptor()`
Methods: `setDepthCompareFunction`, `setDepthWriteEnabled`

**VertexDescriptor** — `new VertexDescriptor()`
Methods: `setAttribute($index, $format, $offset, $bufferIndex)`, `setLayout($index, $stride, $stepFn, $stepRate)`

**SamplerDescriptor** — `new SamplerDescriptor()`
Methods: `setMinFilter`, `setMagFilter`, `setSAddressMode`, `setTAddressMode`

### Data Type Constants

| Constant | Size | Description |
|----------|------|-------------|
| `Metal\FLOAT` | 4 bytes | 32-bit float |
| `Metal\DOUBLE` | 8 bytes | 64-bit float |
| `Metal\INT32` | 4 bytes | Signed 32-bit integer |
| `Metal\UINT32` | 4 bytes | Unsigned 32-bit integer |
| `Metal\INT16` | 2 bytes | Signed 16-bit integer |
| `Metal\UINT16` | 2 bytes | Unsigned 16-bit integer |
| `Metal\INT8` | 1 byte | Signed 8-bit integer |
| `Metal\UINT8` | 1 byte | Unsigned 8-bit integer |

### Other Constants

The extension exposes 100+ Metal enum constants under the `Metal\` namespace:

- **Storage modes**: `StorageModeShared`, `StorageModeManaged`, `StorageModePrivate`, `StorageModeMemoryless`
- **Pixel formats**: `PixelFormatRGBA8Unorm`, `PixelFormatBGRA8Unorm`, `PixelFormatRGBA32Float`, `PixelFormatDepth32Float`, ...
- **Primitive types**: `PrimitiveTypePoint`, `PrimitiveTypeLine`, `PrimitiveTypeTriangle`, `PrimitiveTypeTriangleStrip`
- **Index types**: `IndexTypeUInt16`, `IndexTypeUInt32`
- **Cull modes**: `CullModeNone`, `CullModeFront`, `CullModeBack`
- **Winding**: `WindingClockwise`, `WindingCounterClockwise`
- **Load/store actions**: `LoadActionClear`, `LoadActionLoad`, `StoreActionStore`, `StoreActionDontCare`
- **Compare functions**: `CompareFunctionLess`, `CompareFunctionLessEqual`, `CompareFunctionAlways`, ...
- **Blend factors**: `BlendFactorZero`, `BlendFactorOne`, `BlendFactorSourceAlpha`, `BlendFactorOneMinusSourceAlpha`, ...
- **Texture types**: `TextureType1D`, `TextureType2D`, `TextureType3D`, `TextureTypeCube`
- **Texture usage**: `TextureUsageShaderRead`, `TextureUsageShaderWrite`, `TextureUsageRenderTarget`
- **Vertex formats**: `VertexFormatFloat`, `VertexFormatFloat2`, `VertexFormatFloat3`, `VertexFormatFloat4`, ...
- **Function types**: `FunctionTypeVertex`, `FunctionTypeFragment`, `FunctionTypeKernel`
- **GPU families**: `GPUFamilyApple1`-`GPUFamilyApple9`, `GPUFamilyCommon1`-`GPUFamilyCommon3`, `GPUFamilyMetal3`
- **Sampler filters**: `SamplerMinMagFilterNearest`, `SamplerMinMagFilterLinear`
- **Command buffer status**: `CommandBufferStatusNotEnqueued`, ..., `CommandBufferStatusCompleted`, `CommandBufferStatusError`

## Testing

```bash
make test
```

Runs 15 `.phpt` tests covering device info, buffer I/O, bounds checking, shader compilation, GPU compute dispatch, textures, descriptors, blit encoding, and GPU family detection.

## License

MIT
