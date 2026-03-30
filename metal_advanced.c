/*
  +----------------------------------------------------------------------+
  | php-metal — Advanced Metal API classes                               |
  | Events, Fences, Heaps, Indirect Command Buffers, Argument Encoders,  |
  | Acceleration Structures, Binary Archives, Compute Pass Descriptors   |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "metal_internal.h"

/* ######################################################################
 *  SECTION 1: Events, SharedEvents, Fences
 * ###################################################################### */

static zend_object_handlers metal_event_handlers;
static zend_object_handlers metal_shared_event_handlers;
static zend_object_handlers metal_fence_handlers;

METAL_DEFINE_CREATE_FREE(event,        metal_event_t,        event, metal_ce_event,        &metal_event_handlers)
METAL_DEFINE_CREATE_FREE(shared_event, metal_shared_event_t, event, metal_ce_shared_event, &metal_shared_event_handlers)
METAL_DEFINE_CREATE_FREE(fence,        metal_fence_t,        fence, metal_ce_fence,        &metal_fence_handlers)

/* --- SharedEvent methods --- */

PHP_METHOD(Metal_SharedEvent, getSignaledValue)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_shared_event_t *intern = metal_shared_event_from_obj(Z_OBJ_P(ZEND_THIS));
    RETURN_LONG((zend_long)[intern->event signaledValue]);
}

PHP_METHOD(Metal_SharedEvent, setSignaledValue)
{
    zend_long value;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(value) ZEND_PARSE_PARAMETERS_END();
    metal_shared_event_t *intern = metal_shared_event_from_obj(Z_OBJ_P(ZEND_THIS));
    [intern->event setSignaledValue:(uint64_t)value];
}

/* --- Arginfo --- */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_shared_event_get_value, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_shared_event_set_value, 0, 1, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* --- Method tables --- */
static const zend_function_entry metal_event_methods[]        = { PHP_FE_END };
static const zend_function_entry metal_fence_methods[]         = { PHP_FE_END };
static const zend_function_entry metal_shared_event_methods[]  = {
    PHP_ME(Metal_SharedEvent, getSignaledValue, arginfo_shared_event_get_value, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_SharedEvent, setSignaledValue, arginfo_shared_event_set_value, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

void metal_register_event_classes(void)
{
    METAL_REGISTER_CLASS(event,        "Event",       metal_event_methods,       metal_event_handlers,        metal_ce_event,        ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(shared_event, "SharedEvent", metal_shared_event_methods,metal_shared_event_handlers, metal_ce_shared_event, ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(fence,        "Fence",       metal_fence_methods,       metal_fence_handlers,        metal_ce_fence,        ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
}

/* ######################################################################
 *  SECTION 2: Heaps
 * ###################################################################### */

static zend_object_handlers metal_heap_handlers;
static zend_object_handlers metal_heap_descriptor_handlers;

METAL_DEFINE_CREATE_FREE(heap,            metal_heap_t,            heap,       metal_ce_heap,            &metal_heap_handlers)
METAL_DEFINE_CREATE_FREE(heap_descriptor, metal_heap_descriptor_t, descriptor, metal_ce_heap_descriptor, &metal_heap_descriptor_handlers)

/* --- HeapDescriptor methods --- */

PHP_METHOD(Metal_HeapDescriptor, __construct)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_heap_descriptor_t *intern = metal_heap_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor = [[MTLHeapDescriptor alloc] init];
}

PHP_METHOD(Metal_HeapDescriptor, setSize)
{
    zend_long size;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(size) ZEND_PARSE_PARAMETERS_END();
    metal_heap_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor.size = (NSUInteger)size;
}

PHP_METHOD(Metal_HeapDescriptor, setStorageMode)
{
    zend_long mode;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(mode) ZEND_PARSE_PARAMETERS_END();
    metal_heap_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor.storageMode = (MTLStorageMode)mode;
}

PHP_METHOD(Metal_HeapDescriptor, setCpuCacheMode)
{
    zend_long mode;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(mode) ZEND_PARSE_PARAMETERS_END();
    metal_heap_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor.cpuCacheMode = (MTLCPUCacheMode)mode;
}

PHP_METHOD(Metal_HeapDescriptor, setHazardTrackingMode)
{
    zend_long mode;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(mode) ZEND_PARSE_PARAMETERS_END();
    metal_heap_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor.hazardTrackingMode = (MTLHazardTrackingMode)mode;
}

PHP_METHOD(Metal_HeapDescriptor, setType)
{
    zend_long type;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(type) ZEND_PARSE_PARAMETERS_END();
    metal_heap_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor.type = (MTLHeapType)type;
}

/* --- Heap methods --- */

PHP_METHOD(Metal_Heap, createBuffer)
{
    zend_long length, options = MTLResourceStorageModeShared;
    ZEND_PARSE_PARAMETERS_START(1, 2)
        Z_PARAM_LONG(length)
        Z_PARAM_OPTIONAL Z_PARAM_LONG(options)
    ZEND_PARSE_PARAMETERS_END();

    metal_heap_t *intern = metal_heap_from_obj(Z_OBJ_P(ZEND_THIS));
    id<MTLBuffer> buf = [intern->heap newBufferWithLength:(NSUInteger)length options:(MTLResourceOptions)options];
    if (!buf) { zend_throw_exception(metal_ce_exception, "Failed to create buffer from heap", 0); RETURN_THROWS(); }
    METAL_WRAP_RETURN(buffer, metal_buffer_t, buffer, metal_ce_buffer, buf);
}

PHP_METHOD(Metal_Heap, createTexture)
{
    zval *zdesc;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_OBJECT_OF_CLASS(zdesc, metal_ce_texture_descriptor) ZEND_PARSE_PARAMETERS_END();

    metal_heap_t *intern = metal_heap_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_texture_descriptor_t *desc = metal_texture_descriptor_from_obj(Z_OBJ_P(zdesc));
    id<MTLTexture> tex = [intern->heap newTextureWithDescriptor:desc->descriptor];
    if (!tex) { zend_throw_exception(metal_ce_exception, "Failed to create texture from heap", 0); RETURN_THROWS(); }
    METAL_WRAP_RETURN(texture, metal_texture_t, texture, metal_ce_texture, tex);
}

PHP_METHOD(Metal_Heap, getUsedSize)
{
    ZEND_PARSE_PARAMETERS_NONE();
    RETURN_LONG((zend_long)[metal_heap_from_obj(Z_OBJ_P(ZEND_THIS))->heap usedSize]);
}

PHP_METHOD(Metal_Heap, getCurrentAllocatedSize)
{
    ZEND_PARSE_PARAMETERS_NONE();
    RETURN_LONG((zend_long)[metal_heap_from_obj(Z_OBJ_P(ZEND_THIS))->heap currentAllocatedSize]);
}

PHP_METHOD(Metal_Heap, getMaxAvailableSize)
{
    zend_long alignment;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(alignment) ZEND_PARSE_PARAMETERS_END();
    RETURN_LONG((zend_long)[metal_heap_from_obj(Z_OBJ_P(ZEND_THIS))->heap maxAvailableSizeWithAlignment:(NSUInteger)alignment]);
}

PHP_METHOD(Metal_Heap, setPurgeableState)
{
    zend_long state;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(state) ZEND_PARSE_PARAMETERS_END();
    RETURN_LONG((zend_long)[metal_heap_from_obj(Z_OBJ_P(ZEND_THIS))->heap setPurgeableState:(MTLPurgeableState)state]);
}

/* --- Arginfo --- */
ZEND_BEGIN_ARG_INFO_EX(arginfo_heap_desc_construct, 0, 0, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_heap_set_long, 0, 1, IS_VOID, 0) ZEND_ARG_TYPE_INFO(0, v, IS_LONG, 0) ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_heap_create_buffer, 0, 1, Metal\\Buffer, 0)
    ZEND_ARG_TYPE_INFO(0, length, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_LONG, 0, "0")
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_heap_create_texture, 0, 1, Metal\\Texture, 0)
    ZEND_ARG_OBJ_INFO(0, descriptor, Metal\\TextureDescriptor, 0)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_heap_get_long, 0, 0, IS_LONG, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_heap_max_avail, 0, 1, IS_LONG, 0) ZEND_ARG_TYPE_INFO(0, alignment, IS_LONG, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_heap_set_purgeable, 0, 1, IS_LONG, 0) ZEND_ARG_TYPE_INFO(0, state, IS_LONG, 0) ZEND_END_ARG_INFO()

static const zend_function_entry metal_heap_descriptor_methods[] = {
    PHP_ME(Metal_HeapDescriptor, __construct,          arginfo_heap_desc_construct, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_HeapDescriptor, setSize,              arginfo_heap_set_long,       ZEND_ACC_PUBLIC)
    PHP_ME(Metal_HeapDescriptor, setStorageMode,       arginfo_heap_set_long,       ZEND_ACC_PUBLIC)
    PHP_ME(Metal_HeapDescriptor, setCpuCacheMode,      arginfo_heap_set_long,       ZEND_ACC_PUBLIC)
    PHP_ME(Metal_HeapDescriptor, setHazardTrackingMode,arginfo_heap_set_long,       ZEND_ACC_PUBLIC)
    PHP_ME(Metal_HeapDescriptor, setType,              arginfo_heap_set_long,       ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry metal_heap_methods[] = {
    PHP_ME(Metal_Heap, createBuffer,           arginfo_heap_create_buffer,  ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Heap, createTexture,          arginfo_heap_create_texture, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Heap, getUsedSize,            arginfo_heap_get_long,       ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Heap, getCurrentAllocatedSize, arginfo_heap_get_long,      ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Heap, getMaxAvailableSize,    arginfo_heap_max_avail,      ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Heap, setPurgeableState,      arginfo_heap_set_purgeable,  ZEND_ACC_PUBLIC)
    PHP_FE_END
};

void metal_register_heap_classes(void)
{
    METAL_REGISTER_CLASS(heap_descriptor, "HeapDescriptor", metal_heap_descriptor_methods, metal_heap_descriptor_handlers, metal_ce_heap_descriptor, ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(heap,            "Heap",           metal_heap_methods,            metal_heap_handlers,            metal_ce_heap,            ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
}

/* ######################################################################
 *  SECTION 3: Indirect Command Buffers
 * ###################################################################### */

static zend_object_handlers metal_indirect_command_buffer_handlers;
static zend_object_handlers metal_icb_descriptor_handlers;

METAL_DEFINE_CREATE_FREE(indirect_command_buffer, metal_indirect_command_buffer_t, buffer,     metal_ce_indirect_command_buffer, &metal_indirect_command_buffer_handlers)
METAL_DEFINE_CREATE_FREE(icb_descriptor,          metal_icb_descriptor_t,          descriptor, metal_ce_icb_descriptor,          &metal_icb_descriptor_handlers)

PHP_METHOD(Metal_IndirectCommandBufferDescriptor, __construct)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_icb_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor = [[MTLIndirectCommandBufferDescriptor alloc] init];
}

PHP_METHOD(Metal_IndirectCommandBufferDescriptor, setCommandTypes)
{
    zend_long types;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(types) ZEND_PARSE_PARAMETERS_END();
    metal_icb_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor.commandTypes = (MTLIndirectCommandType)types;
}

PHP_METHOD(Metal_IndirectCommandBufferDescriptor, setInheritBuffers)
{
    bool val;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_BOOL(val) ZEND_PARSE_PARAMETERS_END();
    metal_icb_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor.inheritBuffers = val;
}

PHP_METHOD(Metal_IndirectCommandBufferDescriptor, setInheritPipelineState)
{
    bool val;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_BOOL(val) ZEND_PARSE_PARAMETERS_END();
    metal_icb_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor.inheritPipelineState = val;
}

PHP_METHOD(Metal_IndirectCommandBufferDescriptor, setMaxVertexBufferBindCount)
{
    zend_long count;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(count) ZEND_PARSE_PARAMETERS_END();
    metal_icb_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor.maxVertexBufferBindCount = (NSUInteger)count;
}

PHP_METHOD(Metal_IndirectCommandBufferDescriptor, setMaxFragmentBufferBindCount)
{
    zend_long count;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(count) ZEND_PARSE_PARAMETERS_END();
    metal_icb_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor.maxFragmentBufferBindCount = (NSUInteger)count;
}

PHP_METHOD(Metal_IndirectCommandBuffer, getSize)
{
    ZEND_PARSE_PARAMETERS_NONE();
    RETURN_LONG((zend_long)[metal_indirect_command_buffer_from_obj(Z_OBJ_P(ZEND_THIS))->buffer size]);
}

PHP_METHOD(Metal_IndirectCommandBuffer, resetWithRange)
{
    zend_long offset, length;
    ZEND_PARSE_PARAMETERS_START(2, 2) Z_PARAM_LONG(offset) Z_PARAM_LONG(length) ZEND_PARSE_PARAMETERS_END();
    [metal_indirect_command_buffer_from_obj(Z_OBJ_P(ZEND_THIS))->buffer resetWithRange:NSMakeRange((NSUInteger)offset, (NSUInteger)length)];
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_icb_desc_construct, 0, 0, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_icb_set_long, 0, 1, IS_VOID, 0) ZEND_ARG_TYPE_INFO(0, v, IS_LONG, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_icb_set_bool, 0, 1, IS_VOID, 0) ZEND_ARG_TYPE_INFO(0, v, _IS_BOOL, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_icb_get_size, 0, 0, IS_LONG, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_icb_reset, 0, 2, IS_VOID, 0) ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 0) ZEND_ARG_TYPE_INFO(0, length, IS_LONG, 0) ZEND_END_ARG_INFO()

static const zend_function_entry metal_icb_descriptor_methods[] = {
    PHP_ME(Metal_IndirectCommandBufferDescriptor, __construct,                 arginfo_icb_desc_construct, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_IndirectCommandBufferDescriptor, setCommandTypes,             arginfo_icb_set_long,       ZEND_ACC_PUBLIC)
    PHP_ME(Metal_IndirectCommandBufferDescriptor, setInheritBuffers,           arginfo_icb_set_bool,       ZEND_ACC_PUBLIC)
    PHP_ME(Metal_IndirectCommandBufferDescriptor, setInheritPipelineState,     arginfo_icb_set_bool,       ZEND_ACC_PUBLIC)
    PHP_ME(Metal_IndirectCommandBufferDescriptor, setMaxVertexBufferBindCount, arginfo_icb_set_long,       ZEND_ACC_PUBLIC)
    PHP_ME(Metal_IndirectCommandBufferDescriptor, setMaxFragmentBufferBindCount,arginfo_icb_set_long,      ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry metal_icb_methods[] = {
    PHP_ME(Metal_IndirectCommandBuffer, getSize,       arginfo_icb_get_size, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_IndirectCommandBuffer, resetWithRange, arginfo_icb_reset,   ZEND_ACC_PUBLIC)
    PHP_FE_END
};

void metal_register_indirect_classes(void)
{
    METAL_REGISTER_CLASS(icb_descriptor,          "IndirectCommandBufferDescriptor", metal_icb_descriptor_methods, metal_icb_descriptor_handlers,          metal_ce_icb_descriptor,          ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(indirect_command_buffer,  "IndirectCommandBuffer",          metal_icb_methods,            metal_indirect_command_buffer_handlers,  metal_ce_indirect_command_buffer,  ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
}

/* ######################################################################
 *  SECTION 4: Argument Encoders
 * ###################################################################### */

static zend_object_handlers metal_argument_encoder_handlers;
METAL_DEFINE_CREATE_FREE(argument_encoder, metal_argument_encoder_t, encoder, metal_ce_argument_encoder, &metal_argument_encoder_handlers)

PHP_METHOD(Metal_ArgumentEncoder, getEncodedLength)
{
    ZEND_PARSE_PARAMETERS_NONE();
    RETURN_LONG((zend_long)[metal_argument_encoder_from_obj(Z_OBJ_P(ZEND_THIS))->encoder encodedLength]);
}

PHP_METHOD(Metal_ArgumentEncoder, setArgumentBuffer)
{
    zval *zbuf;
    zend_long offset;
    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_OBJECT_OF_CLASS(zbuf, metal_ce_buffer)
        Z_PARAM_LONG(offset)
    ZEND_PARSE_PARAMETERS_END();

    metal_argument_encoder_t *intern = metal_argument_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_buffer_t *buf = metal_buffer_from_obj(Z_OBJ_P(zbuf));
    [intern->encoder setArgumentBuffer:buf->buffer offset:(NSUInteger)offset];
}

PHP_METHOD(Metal_ArgumentEncoder, setBuffer)
{
    zval *zbuf;
    zend_long offset, index;
    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_OBJECT_OF_CLASS(zbuf, metal_ce_buffer)
        Z_PARAM_LONG(offset)
        Z_PARAM_LONG(index)
    ZEND_PARSE_PARAMETERS_END();

    metal_argument_encoder_t *intern = metal_argument_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_buffer_t *buf = metal_buffer_from_obj(Z_OBJ_P(zbuf));
    [intern->encoder setBuffer:buf->buffer offset:(NSUInteger)offset atIndex:(NSUInteger)index];
}

PHP_METHOD(Metal_ArgumentEncoder, setTexture)
{
    zval *ztex;
    zend_long index;
    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_OBJECT_OF_CLASS(ztex, metal_ce_texture)
        Z_PARAM_LONG(index)
    ZEND_PARSE_PARAMETERS_END();

    metal_argument_encoder_t *intern = metal_argument_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_texture_t *tex = metal_texture_from_obj(Z_OBJ_P(ztex));
    [intern->encoder setTexture:tex->texture atIndex:(NSUInteger)index];
}

PHP_METHOD(Metal_ArgumentEncoder, setSamplerState)
{
    zval *zsmp;
    zend_long index;
    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_OBJECT_OF_CLASS(zsmp, metal_ce_sampler_state)
        Z_PARAM_LONG(index)
    ZEND_PARSE_PARAMETERS_END();

    metal_argument_encoder_t *intern = metal_argument_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_sampler_state_t *smp = metal_sampler_state_from_obj(Z_OBJ_P(zsmp));
    [intern->encoder setSamplerState:smp->state atIndex:(NSUInteger)index];
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_argenc_get_length, 0, 0, IS_LONG, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_argenc_set_argbuf, 0, 2, IS_VOID, 0)
    ZEND_ARG_OBJ_INFO(0, buffer, Metal\\Buffer, 0) ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 0)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_argenc_set_buffer, 0, 3, IS_VOID, 0)
    ZEND_ARG_OBJ_INFO(0, buffer, Metal\\Buffer, 0) ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 0) ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_argenc_set_texture, 0, 2, IS_VOID, 0)
    ZEND_ARG_OBJ_INFO(0, texture, Metal\\Texture, 0) ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_argenc_set_sampler, 0, 2, IS_VOID, 0)
    ZEND_ARG_OBJ_INFO(0, sampler, Metal\\SamplerState, 0) ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry metal_argument_encoder_methods[] = {
    PHP_ME(Metal_ArgumentEncoder, getEncodedLength,  arginfo_argenc_get_length,  ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ArgumentEncoder, setArgumentBuffer,  arginfo_argenc_set_argbuf, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ArgumentEncoder, setBuffer,          arginfo_argenc_set_buffer,  ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ArgumentEncoder, setTexture,         arginfo_argenc_set_texture, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ArgumentEncoder, setSamplerState,    arginfo_argenc_set_sampler, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

void metal_register_argument_encoder_class(void)
{
    METAL_REGISTER_CLASS(argument_encoder, "ArgumentEncoder", metal_argument_encoder_methods, metal_argument_encoder_handlers, metal_ce_argument_encoder, ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
}

/* ######################################################################
 *  SECTION 5: Acceleration Structures (Ray Tracing)
 * ###################################################################### */

static zend_object_handlers metal_acceleration_structure_handlers;
static zend_object_handlers metal_prim_accel_descriptor_handlers;
static zend_object_handlers metal_inst_accel_descriptor_handlers;
static zend_object_handlers metal_triangle_geometry_descriptor_handlers;
static zend_object_handlers metal_bbox_geometry_descriptor_handlers;

METAL_DEFINE_CREATE_FREE(acceleration_structure,        metal_acceleration_structure_t,        accel,      metal_ce_acceleration_structure,        &metal_acceleration_structure_handlers)
METAL_DEFINE_CREATE_FREE(prim_accel_descriptor,         metal_prim_accel_descriptor_t,         descriptor, metal_ce_prim_accel_descriptor,         &metal_prim_accel_descriptor_handlers)
METAL_DEFINE_CREATE_FREE(inst_accel_descriptor,         metal_inst_accel_descriptor_t,         descriptor, metal_ce_inst_accel_descriptor,         &metal_inst_accel_descriptor_handlers)
METAL_DEFINE_CREATE_FREE(triangle_geometry_descriptor,  metal_triangle_geometry_descriptor_t,  descriptor, metal_ce_triangle_geometry_descriptor,  &metal_triangle_geometry_descriptor_handlers)
METAL_DEFINE_CREATE_FREE(bbox_geometry_descriptor,      metal_bbox_geometry_descriptor_t,      descriptor, metal_ce_bbox_geometry_descriptor,      &metal_bbox_geometry_descriptor_handlers)

/* --- AccelerationStructure --- */

PHP_METHOD(Metal_AccelerationStructure, getSize)
{
    ZEND_PARSE_PARAMETERS_NONE();
    RETURN_LONG((zend_long)[metal_acceleration_structure_from_obj(Z_OBJ_P(ZEND_THIS))->accel size]);
}

/* --- PrimitiveAccelerationStructureDescriptor --- */

PHP_METHOD(Metal_PrimitiveAccelerationStructureDescriptor, __construct)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_prim_accel_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor = [[MTLPrimitiveAccelerationStructureDescriptor alloc] init];
}

PHP_METHOD(Metal_PrimitiveAccelerationStructureDescriptor, setGeometryDescriptors)
{
    zval *arr;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_ARRAY(arr) ZEND_PARSE_PARAMETERS_END();

    metal_prim_accel_descriptor_t *intern = metal_prim_accel_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    NSMutableArray *geoms = [NSMutableArray array];

    zval *val;
    ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(arr), val) {
        if (Z_TYPE_P(val) == IS_OBJECT && instanceof_function(Z_OBJCE_P(val), metal_ce_triangle_geometry_descriptor)) {
            [geoms addObject:metal_triangle_geometry_descriptor_from_obj(Z_OBJ_P(val))->descriptor];
        } else if (Z_TYPE_P(val) == IS_OBJECT && instanceof_function(Z_OBJCE_P(val), metal_ce_bbox_geometry_descriptor)) {
            [geoms addObject:metal_bbox_geometry_descriptor_from_obj(Z_OBJ_P(val))->descriptor];
        }
    } ZEND_HASH_FOREACH_END();

    intern->descriptor.geometryDescriptors = geoms;
}

/* --- InstanceAccelerationStructureDescriptor --- */

PHP_METHOD(Metal_InstanceAccelerationStructureDescriptor, __construct)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_inst_accel_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor = [[MTLInstanceAccelerationStructureDescriptor alloc] init];
}

PHP_METHOD(Metal_InstanceAccelerationStructureDescriptor, setInstanceCount)
{
    zend_long count;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(count) ZEND_PARSE_PARAMETERS_END();
    metal_inst_accel_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor.instanceCount = (NSUInteger)count;
}

PHP_METHOD(Metal_InstanceAccelerationStructureDescriptor, setInstanceDescriptorBuffer)
{
    zval *zbuf;
    zend_long offset = 0;
    ZEND_PARSE_PARAMETERS_START(1, 2)
        Z_PARAM_OBJECT_OF_CLASS(zbuf, metal_ce_buffer)
        Z_PARAM_OPTIONAL Z_PARAM_LONG(offset)
    ZEND_PARSE_PARAMETERS_END();

    metal_inst_accel_descriptor_t *intern = metal_inst_accel_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_buffer_t *buf = metal_buffer_from_obj(Z_OBJ_P(zbuf));
    intern->descriptor.instanceDescriptorBuffer = buf->buffer;
    intern->descriptor.instanceDescriptorBufferOffset = (NSUInteger)offset;
}

PHP_METHOD(Metal_InstanceAccelerationStructureDescriptor, setInstancedAccelerationStructures)
{
    zval *arr;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_ARRAY(arr) ZEND_PARSE_PARAMETERS_END();

    metal_inst_accel_descriptor_t *intern = metal_inst_accel_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    NSMutableArray *structs = [NSMutableArray array];
    zval *val;
    ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(arr), val) {
        if (Z_TYPE_P(val) == IS_OBJECT && instanceof_function(Z_OBJCE_P(val), metal_ce_acceleration_structure)) {
            [structs addObject:metal_acceleration_structure_from_obj(Z_OBJ_P(val))->accel];
        }
    } ZEND_HASH_FOREACH_END();
    intern->descriptor.instancedAccelerationStructures = structs;
}

/* --- TriangleGeometryDescriptor --- */

PHP_METHOD(Metal_TriangleGeometryDescriptor, __construct)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_triangle_geometry_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor = [[MTLAccelerationStructureTriangleGeometryDescriptor alloc] init];
}

PHP_METHOD(Metal_TriangleGeometryDescriptor, setVertexBuffer)
{
    zval *zbuf; zend_long offset = 0;
    ZEND_PARSE_PARAMETERS_START(1, 2) Z_PARAM_OBJECT_OF_CLASS(zbuf, metal_ce_buffer) Z_PARAM_OPTIONAL Z_PARAM_LONG(offset) ZEND_PARSE_PARAMETERS_END();
    metal_triangle_geometry_descriptor_t *intern = metal_triangle_geometry_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.vertexBuffer = metal_buffer_from_obj(Z_OBJ_P(zbuf))->buffer;
    intern->descriptor.vertexBufferOffset = (NSUInteger)offset;
}

PHP_METHOD(Metal_TriangleGeometryDescriptor, setVertexStride)
{
    zend_long stride;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(stride) ZEND_PARSE_PARAMETERS_END();
    metal_triangle_geometry_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor.vertexStride = (NSUInteger)stride;
}

PHP_METHOD(Metal_TriangleGeometryDescriptor, setIndexBuffer)
{
    zval *zbuf; zend_long offset = 0;
    ZEND_PARSE_PARAMETERS_START(1, 2) Z_PARAM_OBJECT_OF_CLASS(zbuf, metal_ce_buffer) Z_PARAM_OPTIONAL Z_PARAM_LONG(offset) ZEND_PARSE_PARAMETERS_END();
    metal_triangle_geometry_descriptor_t *intern = metal_triangle_geometry_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.indexBuffer = metal_buffer_from_obj(Z_OBJ_P(zbuf))->buffer;
    intern->descriptor.indexBufferOffset = (NSUInteger)offset;
}

PHP_METHOD(Metal_TriangleGeometryDescriptor, setIndexType)
{
    zend_long type;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(type) ZEND_PARSE_PARAMETERS_END();
    metal_triangle_geometry_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor.indexType = (MTLIndexType)type;
}

PHP_METHOD(Metal_TriangleGeometryDescriptor, setTriangleCount)
{
    zend_long count;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(count) ZEND_PARSE_PARAMETERS_END();
    metal_triangle_geometry_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor.triangleCount = (NSUInteger)count;
}

/* --- BoundingBoxGeometryDescriptor --- */

PHP_METHOD(Metal_BoundingBoxGeometryDescriptor, __construct)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_bbox_geometry_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor = [[MTLAccelerationStructureBoundingBoxGeometryDescriptor alloc] init];
}

PHP_METHOD(Metal_BoundingBoxGeometryDescriptor, setBoundingBoxBuffer)
{
    zval *zbuf; zend_long offset = 0;
    ZEND_PARSE_PARAMETERS_START(1, 2) Z_PARAM_OBJECT_OF_CLASS(zbuf, metal_ce_buffer) Z_PARAM_OPTIONAL Z_PARAM_LONG(offset) ZEND_PARSE_PARAMETERS_END();
    metal_bbox_geometry_descriptor_t *intern = metal_bbox_geometry_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.boundingBoxBuffer = metal_buffer_from_obj(Z_OBJ_P(zbuf))->buffer;
    intern->descriptor.boundingBoxBufferOffset = (NSUInteger)offset;
}

PHP_METHOD(Metal_BoundingBoxGeometryDescriptor, setBoundingBoxStride)
{
    zend_long stride;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(stride) ZEND_PARSE_PARAMETERS_END();
    metal_bbox_geometry_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor.boundingBoxStride = (NSUInteger)stride;
}

PHP_METHOD(Metal_BoundingBoxGeometryDescriptor, setBoundingBoxCount)
{
    zend_long count;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(count) ZEND_PARSE_PARAMETERS_END();
    metal_bbox_geometry_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor.boundingBoxCount = (NSUInteger)count;
}

/* --- Arginfo for acceleration structures --- */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_accel_get_size, 0, 0, IS_LONG, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_accel_construct, 0, 0, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_accel_set_array, 0, 1, IS_VOID, 0) ZEND_ARG_TYPE_INFO(0, arr, IS_ARRAY, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_accel_set_long, 0, 1, IS_VOID, 0) ZEND_ARG_TYPE_INFO(0, v, IS_LONG, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_accel_set_buf, 0, 1, IS_VOID, 0)
    ZEND_ARG_OBJ_INFO(0, buffer, Metal\\Buffer, 0)
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, offset, IS_LONG, 0, "0")
ZEND_END_ARG_INFO()

static const zend_function_entry metal_acceleration_structure_methods[] = {
    PHP_ME(Metal_AccelerationStructure, getSize, arginfo_accel_get_size, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry metal_prim_accel_descriptor_methods[] = {
    PHP_ME(Metal_PrimitiveAccelerationStructureDescriptor, __construct,           arginfo_accel_construct, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_PrimitiveAccelerationStructureDescriptor, setGeometryDescriptors,arginfo_accel_set_array, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry metal_inst_accel_descriptor_methods[] = {
    PHP_ME(Metal_InstanceAccelerationStructureDescriptor, __construct,                          arginfo_accel_construct, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_InstanceAccelerationStructureDescriptor, setInstanceCount,                     arginfo_accel_set_long,  ZEND_ACC_PUBLIC)
    PHP_ME(Metal_InstanceAccelerationStructureDescriptor, setInstanceDescriptorBuffer,          arginfo_accel_set_buf,   ZEND_ACC_PUBLIC)
    PHP_ME(Metal_InstanceAccelerationStructureDescriptor, setInstancedAccelerationStructures,   arginfo_accel_set_array, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry metal_triangle_geometry_descriptor_methods[] = {
    PHP_ME(Metal_TriangleGeometryDescriptor, __construct,    arginfo_accel_construct, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_TriangleGeometryDescriptor, setVertexBuffer,arginfo_accel_set_buf,   ZEND_ACC_PUBLIC)
    PHP_ME(Metal_TriangleGeometryDescriptor, setVertexStride,arginfo_accel_set_long,  ZEND_ACC_PUBLIC)
    PHP_ME(Metal_TriangleGeometryDescriptor, setIndexBuffer, arginfo_accel_set_buf,   ZEND_ACC_PUBLIC)
    PHP_ME(Metal_TriangleGeometryDescriptor, setIndexType,   arginfo_accel_set_long,  ZEND_ACC_PUBLIC)
    PHP_ME(Metal_TriangleGeometryDescriptor, setTriangleCount,arginfo_accel_set_long, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry metal_bbox_geometry_descriptor_methods[] = {
    PHP_ME(Metal_BoundingBoxGeometryDescriptor, __construct,           arginfo_accel_construct, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_BoundingBoxGeometryDescriptor, setBoundingBoxBuffer,  arginfo_accel_set_buf,   ZEND_ACC_PUBLIC)
    PHP_ME(Metal_BoundingBoxGeometryDescriptor, setBoundingBoxStride,  arginfo_accel_set_long,  ZEND_ACC_PUBLIC)
    PHP_ME(Metal_BoundingBoxGeometryDescriptor, setBoundingBoxCount,   arginfo_accel_set_long,  ZEND_ACC_PUBLIC)
    PHP_FE_END
};

void metal_register_acceleration_classes(void)
{
    METAL_REGISTER_CLASS(acceleration_structure,       "AccelerationStructure",                           metal_acceleration_structure_methods,       metal_acceleration_structure_handlers,       metal_ce_acceleration_structure,       ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(prim_accel_descriptor,        "PrimitiveAccelerationStructureDescriptor",        metal_prim_accel_descriptor_methods,        metal_prim_accel_descriptor_handlers,        metal_ce_prim_accel_descriptor,        ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(inst_accel_descriptor,        "InstanceAccelerationStructureDescriptor",         metal_inst_accel_descriptor_methods,        metal_inst_accel_descriptor_handlers,        metal_ce_inst_accel_descriptor,        ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(triangle_geometry_descriptor, "AccelerationStructureTriangleGeometryDescriptor", metal_triangle_geometry_descriptor_methods, metal_triangle_geometry_descriptor_handlers, metal_ce_triangle_geometry_descriptor, ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(bbox_geometry_descriptor,     "AccelerationStructureBoundingBoxGeometryDescriptor", metal_bbox_geometry_descriptor_methods,  metal_bbox_geometry_descriptor_handlers,     metal_ce_bbox_geometry_descriptor,     ZEND_ACC_NO_DYNAMIC_PROPERTIES);
}

/* ######################################################################
 *  SECTION 6: Binary Archives
 * ###################################################################### */

static zend_object_handlers metal_binary_archive_handlers;
static zend_object_handlers metal_binary_archive_descriptor_handlers;

METAL_DEFINE_CREATE_FREE(binary_archive,            metal_binary_archive_t,            archive,    metal_ce_binary_archive,            &metal_binary_archive_handlers)
METAL_DEFINE_CREATE_FREE(binary_archive_descriptor, metal_binary_archive_descriptor_t, descriptor, metal_ce_binary_archive_descriptor, &metal_binary_archive_descriptor_handlers)

PHP_METHOD(Metal_BinaryArchiveDescriptor, __construct)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_binary_archive_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor = [[MTLBinaryArchiveDescriptor alloc] init];
}

PHP_METHOD(Metal_BinaryArchiveDescriptor, setUrl)
{
    zend_string *url;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_STR(url) ZEND_PARSE_PARAMETERS_END();
    metal_binary_archive_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor.url =
        [NSURL fileURLWithPath:[NSString stringWithUTF8String:ZSTR_VAL(url)]];
}

PHP_METHOD(Metal_BinaryArchive, serializeToURL)
{
    zend_string *url;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_STR(url) ZEND_PARSE_PARAMETERS_END();

    metal_binary_archive_t *intern = metal_binary_archive_from_obj(Z_OBJ_P(ZEND_THIS));
    NSError *error = nil;
    BOOL ok = [intern->archive serializeToURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:ZSTR_VAL(url)]]
                                        error:&error];
    if (!ok) {
        zend_throw_exception_ex(metal_ce_exception, 0, "Failed to serialize binary archive: %s",
            [[error localizedDescription] UTF8String]);
        RETURN_THROWS();
    }
}

PHP_METHOD(Metal_BinaryArchive, addComputePipelineFunctions)
{
    zval *zdesc;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_OBJECT_OF_CLASS(zdesc, metal_ce_compute_pipeline_state) ZEND_PARSE_PARAMETERS_END();
    /* For simplicity, we use the compute pipeline descriptor approach — users create descriptors */
    zend_throw_exception(metal_ce_exception, "Use addComputePipelineFunctionsWithDescriptor for pipeline caching", 0);
    RETURN_THROWS();
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_ba_desc_construct, 0, 0, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_ba_set_url, 0, 1, IS_VOID, 0) ZEND_ARG_TYPE_INFO(0, url, IS_STRING, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_ba_serialize, 0, 1, IS_VOID, 0) ZEND_ARG_TYPE_INFO(0, url, IS_STRING, 0) ZEND_END_ARG_INFO()

static const zend_function_entry metal_binary_archive_descriptor_methods[] = {
    PHP_ME(Metal_BinaryArchiveDescriptor, __construct, arginfo_ba_desc_construct, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_BinaryArchiveDescriptor, setUrl,      arginfo_ba_set_url,        ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry metal_binary_archive_methods[] = {
    PHP_ME(Metal_BinaryArchive, serializeToURL, arginfo_ba_serialize, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

void metal_register_binary_archive_classes(void)
{
    METAL_REGISTER_CLASS(binary_archive_descriptor, "BinaryArchiveDescriptor", metal_binary_archive_descriptor_methods, metal_binary_archive_descriptor_handlers, metal_ce_binary_archive_descriptor, ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(binary_archive,            "BinaryArchive",           metal_binary_archive_methods,            metal_binary_archive_handlers,            metal_ce_binary_archive,            ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
}

/* ######################################################################
 *  SECTION 7: Compute Pass Descriptor
 * ###################################################################### */

static zend_object_handlers metal_compute_pass_descriptor_handlers;
METAL_DEFINE_CREATE_FREE(compute_pass_descriptor, metal_compute_pass_descriptor_t, descriptor, metal_ce_compute_pass_descriptor, &metal_compute_pass_descriptor_handlers)

PHP_METHOD(Metal_ComputePassDescriptor, __construct)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_compute_pass_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor = [[MTLComputePassDescriptor alloc] init];
}

PHP_METHOD(Metal_ComputePassDescriptor, setDispatchType)
{
    zend_long type;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(type) ZEND_PARSE_PARAMETERS_END();
    metal_compute_pass_descriptor_from_obj(Z_OBJ_P(ZEND_THIS))->descriptor.dispatchType = (MTLDispatchType)type;
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_cpd_construct, 0, 0, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_cpd_set_long, 0, 1, IS_VOID, 0) ZEND_ARG_TYPE_INFO(0, v, IS_LONG, 0) ZEND_END_ARG_INFO()

static const zend_function_entry metal_compute_pass_descriptor_methods[] = {
    PHP_ME(Metal_ComputePassDescriptor, __construct,    arginfo_cpd_construct, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ComputePassDescriptor, setDispatchType,arginfo_cpd_set_long,  ZEND_ACC_PUBLIC)
    PHP_FE_END
};

void metal_register_compute_pass_descriptor_class(void)
{
    METAL_REGISTER_CLASS(compute_pass_descriptor, "ComputePassDescriptor", metal_compute_pass_descriptor_methods, metal_compute_pass_descriptor_handlers, metal_ce_compute_pass_descriptor, ZEND_ACC_NO_DYNAMIC_PROPERTIES);
}

/* ######################################################################
 *  SECTION 8: Advanced constants
 * ###################################################################### */

void metal_register_advanced_constants(int module_number)
{
    /* Heap types */
    REGISTER_NS_LONG_CONSTANT("Metal", "HeapTypeAutomatic", MTLHeapTypeAutomatic, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "HeapTypePlacement", MTLHeapTypePlacement, CONST_CS | CONST_PERSISTENT);

    /* Purgeable state */
    REGISTER_NS_LONG_CONSTANT("Metal", "PurgeableStateKeepCurrent", MTLPurgeableStateKeepCurrent, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "PurgeableStateNonVolatile", MTLPurgeableStateNonVolatile, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "PurgeableStateVolatile",    MTLPurgeableStateVolatile,    CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "PurgeableStateEmpty",       MTLPurgeableStateEmpty,       CONST_CS | CONST_PERSISTENT);

    /* Hazard tracking modes */
    REGISTER_NS_LONG_CONSTANT("Metal", "HazardTrackingModeDefault",   MTLHazardTrackingModeDefault,   CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "HazardTrackingModeUntracked", MTLHazardTrackingModeUntracked, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "HazardTrackingModeTracked",   MTLHazardTrackingModeTracked,   CONST_CS | CONST_PERSISTENT);

    /* CPU cache modes */
    REGISTER_NS_LONG_CONSTANT("Metal", "CPUCacheModeDefaultCache",  MTLCPUCacheModeDefaultCache,  CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "CPUCacheModeWriteCombined", MTLCPUCacheModeWriteCombined, CONST_CS | CONST_PERSISTENT);

    /* Indirect command types */
    REGISTER_NS_LONG_CONSTANT("Metal", "IndirectCommandTypeDraw",                MTLIndirectCommandTypeDraw,                CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "IndirectCommandTypeDrawIndexed",         MTLIndirectCommandTypeDrawIndexed,         CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "IndirectCommandTypeConcurrentDispatch",  MTLIndirectCommandTypeConcurrentDispatch,  CONST_CS | CONST_PERSISTENT);

    /* Dispatch types */
    REGISTER_NS_LONG_CONSTANT("Metal", "DispatchTypeSerial",     MTLDispatchTypeSerial,     CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "DispatchTypeConcurrent", MTLDispatchTypeConcurrent, CONST_CS | CONST_PERSISTENT);

    /* Resource usage (for useResource) */
    REGISTER_NS_LONG_CONSTANT("Metal", "ResourceUsageRead",   MTLResourceUsageRead,   CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "ResourceUsageWrite",  MTLResourceUsageWrite,  CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "ResourceUsageSample", (1 << 2), CONST_CS | CONST_PERSISTENT); /* deprecated alias for Read */

    /* Barrier scope */
    REGISTER_NS_LONG_CONSTANT("Metal", "BarrierScopeBuffers",  MTLBarrierScopeBuffers,  CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "BarrierScopeTextures", MTLBarrierScopeTextures, CONST_CS | CONST_PERSISTENT);

    /* Render stages */
    REGISTER_NS_LONG_CONSTANT("Metal", "RenderStageVertex",   MTLRenderStageVertex,   CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "RenderStageFragment", MTLRenderStageFragment, CONST_CS | CONST_PERSISTENT);

    /* Acceleration structure usage */
    REGISTER_NS_LONG_CONSTANT("Metal", "AccelerationStructureUsageNone",       MTLAccelerationStructureUsageNone,       CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "AccelerationStructureUsageRefit",      MTLAccelerationStructureUsageRefit,      CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "AccelerationStructureUsagePreferFastBuild", MTLAccelerationStructureUsagePreferFastBuild, CONST_CS | CONST_PERSISTENT);
}
