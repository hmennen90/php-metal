/*
  +----------------------------------------------------------------------+
  | php-metal - PHP Extension for Apple Metal GPU API                    |
  +----------------------------------------------------------------------+
  | Copyright (c) 2026                                                   |
  +----------------------------------------------------------------------+
  | Licensed under the MIT License                                       |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "metal_internal.h"

/* ====================================================================
 *  Class entries (global) — core
 * ==================================================================== */

zend_class_entry *metal_ce_device;
zend_class_entry *metal_ce_buffer;
zend_class_entry *metal_ce_command_queue;
zend_class_entry *metal_ce_command_buffer;
zend_class_entry *metal_ce_compute_encoder;
zend_class_entry *metal_ce_library;
zend_class_entry *metal_ce_metal_function;
zend_class_entry *metal_ce_compute_pipeline_state;
zend_class_entry *metal_ce_render_pipeline_state;
zend_class_entry *metal_ce_render_encoder;
zend_class_entry *metal_ce_texture;
zend_class_entry *metal_ce_texture_descriptor;
zend_class_entry *metal_ce_render_pipeline_descriptor;
zend_class_entry *metal_ce_render_pass_descriptor;
zend_class_entry *metal_ce_color_attachment_descriptor;
zend_class_entry *metal_ce_depth_stencil_state;
zend_class_entry *metal_ce_depth_stencil_descriptor;
zend_class_entry *metal_ce_vertex_descriptor;
zend_class_entry *metal_ce_vertex_buffer_layout_descriptor;
zend_class_entry *metal_ce_vertex_attribute_descriptor;
zend_class_entry *metal_ce_sampler_state;
zend_class_entry *metal_ce_sampler_descriptor;
zend_class_entry *metal_ce_blit_encoder;
zend_class_entry *metal_ce_drawable;
zend_class_entry *metal_ce_exception;

/* Advanced class entries (defined in metal_advanced.c) */
zend_class_entry *metal_ce_event;
zend_class_entry *metal_ce_shared_event;
zend_class_entry *metal_ce_fence;
zend_class_entry *metal_ce_heap;
zend_class_entry *metal_ce_heap_descriptor;
zend_class_entry *metal_ce_indirect_command_buffer;
zend_class_entry *metal_ce_icb_descriptor;
zend_class_entry *metal_ce_argument_encoder;
zend_class_entry *metal_ce_acceleration_structure;
zend_class_entry *metal_ce_prim_accel_descriptor;
zend_class_entry *metal_ce_inst_accel_descriptor;
zend_class_entry *metal_ce_triangle_geometry_descriptor;
zend_class_entry *metal_ce_bbox_geometry_descriptor;
zend_class_entry *metal_ce_binary_archive;
zend_class_entry *metal_ce_binary_archive_descriptor;
zend_class_entry *metal_ce_compute_pass_descriptor;
zend_class_entry *metal_ce_accel_encoder;
zend_class_entry *metal_ce_indirect_render_command;
zend_class_entry *metal_ce_indirect_compute_command;
zend_class_entry *metal_ce_mesh_render_pipeline_descriptor;

/* ====================================================================
 *  Object handler tables
 * ==================================================================== */

static zend_object_handlers metal_device_handlers;
static zend_object_handlers metal_buffer_handlers;
static zend_object_handlers metal_command_queue_handlers;
static zend_object_handlers metal_command_buffer_handlers;
static zend_object_handlers metal_compute_encoder_handlers;
static zend_object_handlers metal_library_handlers;
static zend_object_handlers metal_function_handlers;
static zend_object_handlers metal_compute_pipeline_state_handlers;
static zend_object_handlers metal_render_pipeline_state_handlers;
static zend_object_handlers metal_render_encoder_handlers;
static zend_object_handlers metal_texture_handlers;
static zend_object_handlers metal_texture_descriptor_handlers;
static zend_object_handlers metal_render_pipeline_descriptor_handlers;
static zend_object_handlers metal_render_pass_descriptor_handlers;
static zend_object_handlers metal_color_attachment_descriptor_handlers;
static zend_object_handlers metal_depth_stencil_state_handlers;
static zend_object_handlers metal_depth_stencil_descriptor_handlers;
static zend_object_handlers metal_vertex_descriptor_handlers;
static zend_object_handlers metal_vertex_buffer_layout_descriptor_handlers;
static zend_object_handlers metal_vertex_attribute_descriptor_handlers;
static zend_object_handlers metal_sampler_state_handlers;
static zend_object_handlers metal_sampler_descriptor_handlers;
static zend_object_handlers metal_blit_encoder_handlers;
static zend_object_handlers metal_drawable_handlers;

/* Structs, inline accessors, and macros are in metal_internal.h */

METAL_DEFINE_CREATE_FREE(device,                  metal_device_t,                  device,     metal_ce_device,                  &metal_device_handlers)
METAL_DEFINE_CREATE_FREE(buffer,                  metal_buffer_t,                  buffer,     metal_ce_buffer,                  &metal_buffer_handlers)
METAL_DEFINE_CREATE_FREE(command_queue,            metal_command_queue_t,            queue,      metal_ce_command_queue,            &metal_command_queue_handlers)
METAL_DEFINE_CREATE_FREE(command_buffer,           metal_command_buffer_t,           buffer,     metal_ce_command_buffer,           &metal_command_buffer_handlers)
METAL_DEFINE_CREATE_FREE(compute_encoder,          metal_compute_encoder_t,          encoder,    metal_ce_compute_encoder,          &metal_compute_encoder_handlers)
METAL_DEFINE_CREATE_FREE(library,                  metal_library_t,                  library,    metal_ce_library,                  &metal_library_handlers)
METAL_DEFINE_CREATE_FREE(function,                 metal_function_t,                 function,   metal_ce_metal_function,           &metal_function_handlers)
METAL_DEFINE_CREATE_FREE(compute_pipeline_state,   metal_compute_pipeline_state_t,   state,      metal_ce_compute_pipeline_state,   &metal_compute_pipeline_state_handlers)
METAL_DEFINE_CREATE_FREE(render_pipeline_state,    metal_render_pipeline_state_t,    state,      metal_ce_render_pipeline_state,    &metal_render_pipeline_state_handlers)
METAL_DEFINE_CREATE_FREE(render_encoder,           metal_render_encoder_t,           encoder,    metal_ce_render_encoder,           &metal_render_encoder_handlers)
METAL_DEFINE_CREATE_FREE(texture,                  metal_texture_t,                  texture,    metal_ce_texture,                  &metal_texture_handlers)
METAL_DEFINE_CREATE_FREE(texture_descriptor,       metal_texture_descriptor_t,       descriptor, metal_ce_texture_descriptor,       &metal_texture_descriptor_handlers)
METAL_DEFINE_CREATE_FREE(render_pipeline_descriptor, metal_render_pipeline_descriptor_t, descriptor, metal_ce_render_pipeline_descriptor, &metal_render_pipeline_descriptor_handlers)
METAL_DEFINE_CREATE_FREE(render_pass_descriptor,   metal_render_pass_descriptor_t,   descriptor, metal_ce_render_pass_descriptor,   &metal_render_pass_descriptor_handlers)
METAL_DEFINE_CREATE_FREE(color_attachment_descriptor, metal_color_attachment_descriptor_t, descriptor, metal_ce_color_attachment_descriptor, &metal_color_attachment_descriptor_handlers)
METAL_DEFINE_CREATE_FREE(depth_stencil_state,      metal_depth_stencil_state_t,      state,      metal_ce_depth_stencil_state,      &metal_depth_stencil_state_handlers)
METAL_DEFINE_CREATE_FREE(depth_stencil_descriptor, metal_depth_stencil_descriptor_t, descriptor, metal_ce_depth_stencil_descriptor, &metal_depth_stencil_descriptor_handlers)
METAL_DEFINE_CREATE_FREE(vertex_descriptor,        metal_vertex_descriptor_t,        descriptor, metal_ce_vertex_descriptor,        &metal_vertex_descriptor_handlers)
METAL_DEFINE_CREATE_FREE(vertex_buffer_layout_descriptor, metal_vertex_buffer_layout_descriptor_t, descriptor, metal_ce_vertex_buffer_layout_descriptor, &metal_vertex_buffer_layout_descriptor_handlers)
METAL_DEFINE_CREATE_FREE(vertex_attribute_descriptor, metal_vertex_attribute_descriptor_t, descriptor, metal_ce_vertex_attribute_descriptor, &metal_vertex_attribute_descriptor_handlers)
METAL_DEFINE_CREATE_FREE(sampler_state,            metal_sampler_state_t,            state,      metal_ce_sampler_state,            &metal_sampler_state_handlers)
METAL_DEFINE_CREATE_FREE(sampler_descriptor,       metal_sampler_descriptor_t,       descriptor, metal_ce_sampler_descriptor,       &metal_sampler_descriptor_handlers)
METAL_DEFINE_CREATE_FREE(blit_encoder,             metal_blit_encoder_t,             encoder,    metal_ce_blit_encoder,             &metal_blit_encoder_handlers)
METAL_DEFINE_CREATE_FREE(drawable,                 metal_drawable_t,                 drawable,   metal_ce_drawable,                 &metal_drawable_handlers)

/* ====================================================================
 *  Metal\GPU — free function
 * ==================================================================== */

/* {{{ proto Metal\Device Metal\createSystemDefaultDevice() */
PHP_FUNCTION(Metal_createSystemDefaultDevice)
{
    ZEND_PARSE_PARAMETERS_NONE();

    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    if (device == nil) {
        zend_throw_exception(metal_ce_exception, "No Metal-capable GPU found", 0);
        RETURN_THROWS();
    }

    METAL_WRAP_RETURN(device, metal_device_t, device, metal_ce_device, device);
}
/* }}} */

/* {{{ proto array Metal\allDevices() */
PHP_FUNCTION(Metal_allDevices)
{
    ZEND_PARSE_PARAMETERS_NONE();

    NSArray<id<MTLDevice>> *devices = MTLCopyAllDevices();
    if (devices == nil || [devices count] == 0) {
        array_init(return_value);
        return;
    }

    array_init_size(return_value, (uint32_t)[devices count]);

    for (id<MTLDevice> dev in devices) {
        zval zdev;
        object_init_ex(&zdev, metal_ce_device);
        metal_device_t *intern = metal_device_from_obj(Z_OBJ(zdev));
        intern->device = dev;
        zend_hash_next_index_insert(Z_ARRVAL_P(return_value), &zdev);
    }
}
/* }}} */

/* ====================================================================
 *  Metal\Device methods
 * ==================================================================== */

/* {{{ proto string Metal\Device::getName() */
PHP_METHOD(Metal_Device, getName)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_device_t *intern = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    NSString *name = [intern->device name];
    RETURN_STRING([name UTF8String]);
}
/* }}} */

/* {{{ proto bool Metal\Device::isLowPower() */
PHP_METHOD(Metal_Device, isLowPower)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_device_t *intern = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    RETURN_BOOL([intern->device isLowPower]);
}
/* }}} */

/* {{{ proto bool Metal\Device::isRemovable() */
PHP_METHOD(Metal_Device, isRemovable)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_device_t *intern = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    RETURN_BOOL([intern->device isRemovable]);
}
/* }}} */

/* {{{ proto int Metal\Device::getRegistryID() */
PHP_METHOD(Metal_Device, getRegistryID)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_device_t *intern = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    RETURN_LONG((zend_long)[intern->device registryID]);
}
/* }}} */

/* {{{ proto int Metal\Device::getMaxThreadsPerThreadgroup() */
PHP_METHOD(Metal_Device, getMaxThreadsPerThreadgroup)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_device_t *intern = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    MTLSize size = [intern->device maxThreadsPerThreadgroup];
    array_init_size(return_value, 3);
    add_next_index_long(return_value, (zend_long)size.width);
    add_next_index_long(return_value, (zend_long)size.height);
    add_next_index_long(return_value, (zend_long)size.depth);
}
/* }}} */

/* {{{ proto int Metal\Device::getMaxThreadgroupMemoryLength() */
PHP_METHOD(Metal_Device, getMaxThreadgroupMemoryLength)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_device_t *intern = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    RETURN_LONG((zend_long)[intern->device maxThreadgroupMemoryLength]);
}
/* }}} */

/* {{{ proto int Metal\Device::getMaxBufferLength() */
PHP_METHOD(Metal_Device, getMaxBufferLength)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_device_t *intern = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    RETURN_LONG((zend_long)[intern->device maxBufferLength]);
}
/* }}} */

/* {{{ proto bool Metal\Device::supportsFamily(int $family) */
PHP_METHOD(Metal_Device, supportsFamily)
{
    zend_long family;
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(family)
    ZEND_PARSE_PARAMETERS_END();

    metal_device_t *intern = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    RETURN_BOOL([intern->device supportsFamily:(MTLGPUFamily)family]);
}
/* }}} */

/* {{{ proto Metal\Buffer Metal\Device::createBuffer(int $length, int $options = 0) */
PHP_METHOD(Metal_Device, createBuffer)
{
    zend_long length;
    zend_long options = MTLResourceStorageModeShared;

    ZEND_PARSE_PARAMETERS_START(1, 2)
        Z_PARAM_LONG(length)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(options)
    ZEND_PARSE_PARAMETERS_END();

    if (length <= 0) {
        zend_throw_exception(metal_ce_exception, "Buffer length must be positive", 0);
        RETURN_THROWS();
    }

    metal_device_t *intern = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    id<MTLBuffer> buf = [intern->device newBufferWithLength:(NSUInteger)length
                                                   options:(MTLResourceOptions)options];
    if (buf == nil) {
        zend_throw_exception(metal_ce_exception, "Failed to create buffer", 0);
        RETURN_THROWS();
    }

    METAL_WRAP_RETURN(buffer, metal_buffer_t, buffer, metal_ce_buffer, buf);
}
/* }}} */

/* {{{ proto Metal\Buffer Metal\Device::createBufferFromData(array $data, int $type = Metal\FLOAT, int $options = 0) */
PHP_METHOD(Metal_Device, createBufferFromData)
{
    zval *arr;
    zend_long dtype = METAL_DTYPE_FLOAT;
    zend_long options = MTLResourceStorageModeShared;

    ZEND_PARSE_PARAMETERS_START(1, 3)
        Z_PARAM_ARRAY(arr)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(dtype)
        Z_PARAM_LONG(options)
    ZEND_PARSE_PARAMETERS_END();

    HashTable *ht = Z_ARRVAL_P(arr);
    uint32_t count = zend_hash_num_elements(ht);
    if (count == 0) {
        zend_throw_exception(metal_ce_exception, "Data array must not be empty", 0);
        RETURN_THROWS();
    }

    size_t elem_size;
    switch (dtype) {
        case METAL_DTYPE_FLOAT:   elem_size = sizeof(float);    break;
        case METAL_DTYPE_DOUBLE:  elem_size = sizeof(double);   break;
        case METAL_DTYPE_INT32:   elem_size = sizeof(int32_t);  break;
        case METAL_DTYPE_UINT32:  elem_size = sizeof(uint32_t); break;
        case METAL_DTYPE_INT16:   elem_size = sizeof(int16_t);  break;
        case METAL_DTYPE_UINT16:  elem_size = sizeof(uint16_t); break;
        case METAL_DTYPE_INT8:    elem_size = sizeof(int8_t);   break;
        case METAL_DTYPE_UINT8:   elem_size = sizeof(uint8_t);  break;
        default:
            zend_throw_exception(metal_ce_exception, "Unknown data type", 0);
            RETURN_THROWS();
    }

    size_t total = elem_size * count;
    metal_device_t *intern = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));

    id<MTLBuffer> buf = [intern->device newBufferWithLength:total
                                                   options:(MTLResourceOptions)options];
    if (buf == nil) {
        zend_throw_exception(metal_ce_exception, "Failed to create buffer", 0);
        RETURN_THROWS();
    }

    void *contents = [buf contents];
    uint32_t i = 0;
    zval *val;
    ZEND_HASH_FOREACH_VAL(ht, val) {
        switch (dtype) {
            case METAL_DTYPE_FLOAT:   ((float *)contents)[i]    = (float)zval_get_double(val);    break;
            case METAL_DTYPE_DOUBLE:  ((double *)contents)[i]   = zval_get_double(val);           break;
            case METAL_DTYPE_INT32:   ((int32_t *)contents)[i]  = (int32_t)zval_get_long(val);    break;
            case METAL_DTYPE_UINT32:  ((uint32_t *)contents)[i] = (uint32_t)zval_get_long(val);   break;
            case METAL_DTYPE_INT16:   ((int16_t *)contents)[i]  = (int16_t)zval_get_long(val);    break;
            case METAL_DTYPE_UINT16:  ((uint16_t *)contents)[i] = (uint16_t)zval_get_long(val);   break;
            case METAL_DTYPE_INT8:    ((int8_t *)contents)[i]   = (int8_t)zval_get_long(val);     break;
            case METAL_DTYPE_UINT8:   ((uint8_t *)contents)[i]  = (uint8_t)zval_get_long(val);    break;
        }
        i++;
    } ZEND_HASH_FOREACH_END();

    METAL_WRAP_RETURN(buffer, metal_buffer_t, buffer, metal_ce_buffer, buf);
}
/* }}} */

/* {{{ proto Metal\CommandQueue Metal\Device::createCommandQueue() */
PHP_METHOD(Metal_Device, createCommandQueue)
{
    ZEND_PARSE_PARAMETERS_NONE();

    metal_device_t *intern = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    id<MTLCommandQueue> queue = [intern->device newCommandQueue];
    if (queue == nil) {
        zend_throw_exception(metal_ce_exception, "Failed to create command queue", 0);
        RETURN_THROWS();
    }

    METAL_WRAP_RETURN(command_queue, metal_command_queue_t, queue, metal_ce_command_queue, queue);
}
/* }}} */

/* {{{ proto Metal\Library Metal\Device::createLibraryWithSource(string $source, array $options = []) */
PHP_METHOD(Metal_Device, createLibraryWithSource)
{
    zend_string *source;
    zval *zoptions = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 2)
        Z_PARAM_STR(source)
        Z_PARAM_OPTIONAL
        Z_PARAM_ARRAY_OR_NULL(zoptions)
    ZEND_PARSE_PARAMETERS_END();

    metal_device_t *intern = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));

    NSString *src = [NSString stringWithUTF8String:ZSTR_VAL(source)];
    MTLCompileOptions *opts = [[MTLCompileOptions alloc] init];

    if (zoptions != NULL) {
        HashTable *ht = Z_ARRVAL_P(zoptions);
        zval *val;

        if ((val = zend_hash_str_find(ht, "fastMathEnabled", sizeof("fastMathEnabled") - 1)) != NULL) {
#if defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && __MAC_OS_X_VERSION_MAX_ALLOWED >= 150000
            opts.mathMode = zend_is_true(val) ? MTLMathModeFast : MTLMathModeSafe;
#else
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wdeprecated-declarations"
            opts.fastMathEnabled = zend_is_true(val);
            #pragma clang diagnostic pop
#endif
        }
        if ((val = zend_hash_str_find(ht, "languageVersion", sizeof("languageVersion") - 1)) != NULL) {
            opts.languageVersion = (MTLLanguageVersion)zval_get_long(val);
        }
    }

    NSError *error = nil;
    id<MTLLibrary> library = [intern->device newLibraryWithSource:src options:opts error:&error];

    if (library == nil) {
        NSString *desc = [error localizedDescription];
        zend_throw_exception_ex(metal_ce_exception, 0,
            "Metal shader compilation failed: %s", [desc UTF8String]);
        RETURN_THROWS();
    }

    METAL_WRAP_RETURN(library, metal_library_t, library, metal_ce_library, library);
}
/* }}} */

/* {{{ proto Metal\Library Metal\Device::createLibraryWithFile(string $path) */
PHP_METHOD(Metal_Device, createLibraryWithFile)
{
    zend_string *path;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STR(path)
    ZEND_PARSE_PARAMETERS_END();

    metal_device_t *intern = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    NSString *nspath = [NSString stringWithUTF8String:ZSTR_VAL(path)];
    NSError *error = nil;

    id<MTLLibrary> library = [intern->device newLibraryWithURL:[NSURL fileURLWithPath:nspath]
                                                         error:&error];
    if (library == nil) {
        NSString *desc = [error localizedDescription];
        zend_throw_exception_ex(metal_ce_exception, 0,
            "Failed to load Metal library from file: %s", [desc UTF8String]);
        RETURN_THROWS();
    }

    METAL_WRAP_RETURN(library, metal_library_t, library, metal_ce_library, library);
}
/* }}} */

/* {{{ proto Metal\Library Metal\Device::createDefaultLibrary() */
PHP_METHOD(Metal_Device, createDefaultLibrary)
{
    ZEND_PARSE_PARAMETERS_NONE();

    metal_device_t *intern = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    id<MTLLibrary> library = [intern->device newDefaultLibrary];

    if (library == nil) {
        zend_throw_exception(metal_ce_exception, "No default Metal library found", 0);
        RETURN_THROWS();
    }

    METAL_WRAP_RETURN(library, metal_library_t, library, metal_ce_library, library);
}
/* }}} */

/* {{{ proto Metal\ComputePipelineState Metal\Device::createComputePipelineState(Metal\MetalFunction $function) */
PHP_METHOD(Metal_Device, createComputePipelineState)
{
    zval *zfunction;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zfunction, metal_ce_metal_function)
    ZEND_PARSE_PARAMETERS_END();

    metal_device_t *dev = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_function_t *fn = metal_function_from_obj(Z_OBJ_P(zfunction));

    NSError *error = nil;
    id<MTLComputePipelineState> state = [dev->device newComputePipelineStateWithFunction:fn->function
                                                                                  error:&error];
    if (state == nil) {
        NSString *desc = [error localizedDescription];
        zend_throw_exception_ex(metal_ce_exception, 0,
            "Failed to create compute pipeline state: %s", [desc UTF8String]);
        RETURN_THROWS();
    }

    METAL_WRAP_RETURN(compute_pipeline_state, metal_compute_pipeline_state_t, state,
                      metal_ce_compute_pipeline_state, state);
}
/* }}} */

/* {{{ proto Metal\Texture Metal\Device::createTexture(Metal\TextureDescriptor $descriptor) */
PHP_METHOD(Metal_Device, createTexture)
{
    zval *zdescriptor;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zdescriptor, metal_ce_texture_descriptor)
    ZEND_PARSE_PARAMETERS_END();

    metal_device_t *dev = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_texture_descriptor_t *desc = metal_texture_descriptor_from_obj(Z_OBJ_P(zdescriptor));

    id<MTLTexture> texture = [dev->device newTextureWithDescriptor:desc->descriptor];
    if (texture == nil) {
        zend_throw_exception(metal_ce_exception, "Failed to create texture", 0);
        RETURN_THROWS();
    }

    METAL_WRAP_RETURN(texture, metal_texture_t, texture, metal_ce_texture, texture);
}
/* }}} */

/* {{{ proto Metal\DepthStencilState Metal\Device::createDepthStencilState(Metal\DepthStencilDescriptor $descriptor) */
PHP_METHOD(Metal_Device, createDepthStencilState)
{
    zval *zdescriptor;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zdescriptor, metal_ce_depth_stencil_descriptor)
    ZEND_PARSE_PARAMETERS_END();

    metal_device_t *dev = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_depth_stencil_descriptor_t *desc = metal_depth_stencil_descriptor_from_obj(Z_OBJ_P(zdescriptor));

    id<MTLDepthStencilState> state = [dev->device newDepthStencilStateWithDescriptor:desc->descriptor];
    if (state == nil) {
        zend_throw_exception(metal_ce_exception, "Failed to create depth stencil state", 0);
        RETURN_THROWS();
    }

    METAL_WRAP_RETURN(depth_stencil_state, metal_depth_stencil_state_t, state,
                      metal_ce_depth_stencil_state, state);
}
/* }}} */

/* {{{ proto Metal\RenderPipelineState Metal\Device::createRenderPipelineState(Metal\RenderPipelineDescriptor $descriptor) */
PHP_METHOD(Metal_Device, createRenderPipelineState)
{
    zval *zdescriptor;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zdescriptor, metal_ce_render_pipeline_descriptor)
    ZEND_PARSE_PARAMETERS_END();

    metal_device_t *dev = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_render_pipeline_descriptor_t *desc = metal_render_pipeline_descriptor_from_obj(Z_OBJ_P(zdescriptor));

    NSError *error = nil;
    id<MTLRenderPipelineState> state = [dev->device newRenderPipelineStateWithDescriptor:desc->descriptor
                                                                                  error:&error];
    if (state == nil) {
        NSString *d = [error localizedDescription];
        zend_throw_exception_ex(metal_ce_exception, 0,
            "Failed to create render pipeline state: %s", [d UTF8String]);
        RETURN_THROWS();
    }

    METAL_WRAP_RETURN(render_pipeline_state, metal_render_pipeline_state_t, state,
                      metal_ce_render_pipeline_state, state);
}
/* }}} */

/* {{{ proto Metal\SamplerState Metal\Device::createSamplerState(Metal\SamplerDescriptor $descriptor) */
PHP_METHOD(Metal_Device, createSamplerState)
{
    zval *zdescriptor;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zdescriptor, metal_ce_sampler_descriptor)
    ZEND_PARSE_PARAMETERS_END();

    metal_device_t *dev = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_sampler_descriptor_t *desc = metal_sampler_descriptor_from_obj(Z_OBJ_P(zdescriptor));

    id<MTLSamplerState> state = [dev->device newSamplerStateWithDescriptor:desc->descriptor];
    if (state == nil) {
        zend_throw_exception(metal_ce_exception, "Failed to create sampler state", 0);
        RETURN_THROWS();
    }

    METAL_WRAP_RETURN(sampler_state, metal_sampler_state_t, state,
                      metal_ce_sampler_state, state);
}
/* }}} */

/* {{{ Advanced Device factory methods */

PHP_METHOD(Metal_Device, createEvent)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_device_t *dev = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    id<MTLEvent> event = [dev->device newEvent];
    if (!event) { zend_throw_exception(metal_ce_exception, "Failed to create event", 0); RETURN_THROWS(); }
    METAL_WRAP_RETURN(event, metal_event_t, event, metal_ce_event, event);
}

PHP_METHOD(Metal_Device, createSharedEvent)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_device_t *dev = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    id<MTLSharedEvent> event = [dev->device newSharedEvent];
    if (!event) { zend_throw_exception(metal_ce_exception, "Failed to create shared event", 0); RETURN_THROWS(); }
    METAL_WRAP_RETURN(shared_event, metal_shared_event_t, event, metal_ce_shared_event, event);
}

PHP_METHOD(Metal_Device, createFence)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_device_t *dev = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    id<MTLFence> fence = [dev->device newFence];
    if (!fence) { zend_throw_exception(metal_ce_exception, "Failed to create fence", 0); RETURN_THROWS(); }
    METAL_WRAP_RETURN(fence, metal_fence_t, fence, metal_ce_fence, fence);
}

PHP_METHOD(Metal_Device, createHeap)
{
    zval *zdesc;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_OBJECT_OF_CLASS(zdesc, metal_ce_heap_descriptor) ZEND_PARSE_PARAMETERS_END();
    metal_device_t *dev = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_heap_descriptor_t *desc = metal_heap_descriptor_from_obj(Z_OBJ_P(zdesc));
    id<MTLHeap> heap = [dev->device newHeapWithDescriptor:desc->descriptor];
    if (!heap) { zend_throw_exception(metal_ce_exception, "Failed to create heap", 0); RETURN_THROWS(); }
    METAL_WRAP_RETURN(heap, metal_heap_t, heap, metal_ce_heap, heap);
}

PHP_METHOD(Metal_Device, createIndirectCommandBuffer)
{
    zval *zdesc;
    zend_long max_count;
    zend_long options = 0;
    ZEND_PARSE_PARAMETERS_START(2, 3)
        Z_PARAM_OBJECT_OF_CLASS(zdesc, metal_ce_icb_descriptor)
        Z_PARAM_LONG(max_count)
        Z_PARAM_OPTIONAL Z_PARAM_LONG(options)
    ZEND_PARSE_PARAMETERS_END();

    metal_device_t *dev = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_icb_descriptor_t *desc = metal_icb_descriptor_from_obj(Z_OBJ_P(zdesc));
    id<MTLIndirectCommandBuffer> icb = [dev->device newIndirectCommandBufferWithDescriptor:desc->descriptor
                                                                           maxCommandCount:(NSUInteger)max_count
                                                                                   options:(MTLResourceOptions)options];
    if (!icb) { zend_throw_exception(metal_ce_exception, "Failed to create indirect command buffer", 0); RETURN_THROWS(); }
    METAL_WRAP_RETURN(indirect_command_buffer, metal_indirect_command_buffer_t, buffer, metal_ce_indirect_command_buffer, icb);
}

PHP_METHOD(Metal_Device, createBinaryArchive)
{
    zval *zdesc;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_OBJECT_OF_CLASS(zdesc, metal_ce_binary_archive_descriptor) ZEND_PARSE_PARAMETERS_END();
    metal_device_t *dev = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_binary_archive_descriptor_t *desc = metal_binary_archive_descriptor_from_obj(Z_OBJ_P(zdesc));
    NSError *error = nil;
    id<MTLBinaryArchive> archive = [dev->device newBinaryArchiveWithDescriptor:desc->descriptor error:&error];
    if (!archive) {
        zend_throw_exception_ex(metal_ce_exception, 0, "Failed to create binary archive: %s", [[error localizedDescription] UTF8String]);
        RETURN_THROWS();
    }
    METAL_WRAP_RETURN(binary_archive, metal_binary_archive_t, archive, metal_ce_binary_archive, archive);
}

PHP_METHOD(Metal_Device, createAccelerationStructureWithSize)
{
    zend_long size;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(size) ZEND_PARSE_PARAMETERS_END();
    metal_device_t *dev = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    MTLAccelerationStructureDescriptor *desc = [[MTLPrimitiveAccelerationStructureDescriptor alloc] init];
    id<MTLAccelerationStructure> accel = [dev->device newAccelerationStructureWithSize:(NSUInteger)size];
    if (!accel) { zend_throw_exception(metal_ce_exception, "Failed to create acceleration structure", 0); RETURN_THROWS(); }
    (void)desc;
    METAL_WRAP_RETURN(acceleration_structure, metal_acceleration_structure_t, accel, metal_ce_acceleration_structure, accel);
}

PHP_METHOD(Metal_Device, getAccelerationStructureSizes)
{
    zval *zdesc;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_OBJECT(zdesc) ZEND_PARSE_PARAMETERS_END();

    metal_device_t *dev = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    MTLAccelerationStructureDescriptor *desc = nil;

    if (instanceof_function(Z_OBJCE_P(zdesc), metal_ce_prim_accel_descriptor)) {
        desc = metal_prim_accel_descriptor_from_obj(Z_OBJ_P(zdesc))->descriptor;
    } else if (instanceof_function(Z_OBJCE_P(zdesc), metal_ce_inst_accel_descriptor)) {
        desc = (MTLAccelerationStructureDescriptor *)metal_inst_accel_descriptor_from_obj(Z_OBJ_P(zdesc))->descriptor;
    } else {
        zend_throw_exception(metal_ce_exception, "Expected acceleration structure descriptor", 0);
        RETURN_THROWS();
    }

    MTLAccelerationStructureSizes sizes = [dev->device accelerationStructureSizesWithDescriptor:desc];
    array_init_size(return_value, 3);
    add_assoc_long(return_value, "accelerationStructureSize", (zend_long)sizes.accelerationStructureSize);
    add_assoc_long(return_value, "buildScratchBufferSize", (zend_long)sizes.buildScratchBufferSize);
    add_assoc_long(return_value, "refitScratchBufferSize", (zend_long)sizes.refitScratchBufferSize);
}

PHP_METHOD(Metal_Device, heapBufferSizeAndAlign)
{
    zend_long length, options = 0;
    ZEND_PARSE_PARAMETERS_START(1, 2) Z_PARAM_LONG(length) Z_PARAM_OPTIONAL Z_PARAM_LONG(options) ZEND_PARSE_PARAMETERS_END();
    metal_device_t *dev = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    MTLSizeAndAlign sa = [dev->device heapBufferSizeAndAlignWithLength:(NSUInteger)length options:(MTLResourceOptions)options];
    array_init_size(return_value, 2);
    add_assoc_long(return_value, "size", (zend_long)sa.size);
    add_assoc_long(return_value, "align", (zend_long)sa.align);
}

PHP_METHOD(Metal_Device, heapTextureSizeAndAlign)
{
    zval *zdesc;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_OBJECT_OF_CLASS(zdesc, metal_ce_texture_descriptor) ZEND_PARSE_PARAMETERS_END();
    metal_device_t *dev = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_texture_descriptor_t *desc = metal_texture_descriptor_from_obj(Z_OBJ_P(zdesc));
    MTLSizeAndAlign sa = [dev->device heapTextureSizeAndAlignWithDescriptor:desc->descriptor];
    array_init_size(return_value, 2);
    add_assoc_long(return_value, "size", (zend_long)sa.size);
    add_assoc_long(return_value, "align", (zend_long)sa.align);
}

PHP_METHOD(Metal_Device, createRenderPipelineStateWithMeshDescriptor)
{
    zval *zdesc;
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zdesc, metal_ce_mesh_render_pipeline_descriptor)
    ZEND_PARSE_PARAMETERS_END();

    metal_device_t *dev = metal_device_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_mesh_render_pipeline_descriptor_t *desc = metal_mesh_render_pipeline_descriptor_from_obj(Z_OBJ_P(zdesc));
    NSError *error = nil;
    id<MTLRenderPipelineState> state = [dev->device newRenderPipelineStateWithMeshDescriptor:desc->descriptor
                                                                                    options:0
                                                                                 reflection:nil
                                                                                      error:&error];
    if (!state) {
        zend_throw_exception_ex(metal_ce_exception, 0, "Failed to create mesh render pipeline: %s", [[error localizedDescription] UTF8String]);
        RETURN_THROWS();
    }
    METAL_WRAP_RETURN(render_pipeline_state, metal_render_pipeline_state_t, state, metal_ce_render_pipeline_state, state);
}

PHP_METHOD(Metal_Device, createBufferFromTexture)
{
    zval *ztex;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_OBJECT_OF_CLASS(ztex, metal_ce_texture) ZEND_PARSE_PARAMETERS_END();
    /* Not directly available — users should use blit encoder to copy texture to buffer */
    zend_throw_exception(metal_ce_exception, "Use BlitCommandEncoder to copy texture data to a buffer", 0);
    RETURN_THROWS();
}

/* }}} */

/* ====================================================================
 *  Metal\Buffer methods
 * ==================================================================== */

/* {{{ proto int Metal\Buffer::getLength() */
PHP_METHOD(Metal_Buffer, getLength)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_buffer_t *intern = metal_buffer_from_obj(Z_OBJ_P(ZEND_THIS));
    RETURN_LONG((zend_long)[intern->buffer length]);
}
/* }}} */

/* {{{ proto array Metal\Buffer::getContents(int $type = METAL_FLOAT, int $count = -1, int $offset = 0) */
PHP_METHOD(Metal_Buffer, getContents)
{
    zend_long dtype = METAL_DTYPE_FLOAT;
    zend_long count = -1;
    zend_long offset = 0;

    ZEND_PARSE_PARAMETERS_START(0, 3)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(dtype)
        Z_PARAM_LONG(count)
        Z_PARAM_LONG(offset)
    ZEND_PARSE_PARAMETERS_END();

    metal_buffer_t *intern = metal_buffer_from_obj(Z_OBJ_P(ZEND_THIS));
    NSUInteger length = [intern->buffer length];

    size_t elem_size;
    switch (dtype) {
        case METAL_DTYPE_FLOAT:   elem_size = sizeof(float);    break;
        case METAL_DTYPE_DOUBLE:  elem_size = sizeof(double);   break;
        case METAL_DTYPE_INT32:   elem_size = sizeof(int32_t);  break;
        case METAL_DTYPE_UINT32:  elem_size = sizeof(uint32_t); break;
        case METAL_DTYPE_INT16:   elem_size = sizeof(int16_t);  break;
        case METAL_DTYPE_UINT16:  elem_size = sizeof(uint16_t); break;
        case METAL_DTYPE_INT8:    elem_size = sizeof(int8_t);   break;
        case METAL_DTYPE_UINT8:   elem_size = sizeof(uint8_t);  break;
        default:
            zend_throw_exception(metal_ce_exception, "Unknown data type", 0);
            RETURN_THROWS();
    }

    if (count < 0) {
        count = (zend_long)((length - (NSUInteger)offset) / elem_size);
    }

    if ((NSUInteger)offset + (NSUInteger)count * elem_size > length) {
        zend_throw_exception(metal_ce_exception, "Read would exceed buffer bounds", 0);
        RETURN_THROWS();
    }

    void *contents = (char *)[intern->buffer contents] + offset;
    array_init_size(return_value, (uint32_t)count);

    for (zend_long i = 0; i < count; i++) {
        switch (dtype) {
            case METAL_DTYPE_FLOAT:   add_next_index_double(return_value, ((float *)contents)[i]);            break;
            case METAL_DTYPE_DOUBLE:  add_next_index_double(return_value, ((double *)contents)[i]);           break;
            case METAL_DTYPE_INT32:   add_next_index_long(return_value, ((int32_t *)contents)[i]);            break;
            case METAL_DTYPE_UINT32:  add_next_index_long(return_value, ((uint32_t *)contents)[i]);           break;
            case METAL_DTYPE_INT16:   add_next_index_long(return_value, ((int16_t *)contents)[i]);            break;
            case METAL_DTYPE_UINT16:  add_next_index_long(return_value, ((uint16_t *)contents)[i]);           break;
            case METAL_DTYPE_INT8:    add_next_index_long(return_value, ((int8_t *)contents)[i]);             break;
            case METAL_DTYPE_UINT8:   add_next_index_long(return_value, ((uint8_t *)contents)[i]);            break;
        }
    }
}
/* }}} */

/* {{{ proto void Metal\Buffer::writeContents(array $data, int $type = METAL_FLOAT, int $offset = 0) */
PHP_METHOD(Metal_Buffer, writeContents)
{
    zval *arr;
    zend_long dtype = METAL_DTYPE_FLOAT;
    zend_long byte_offset = 0;

    ZEND_PARSE_PARAMETERS_START(1, 3)
        Z_PARAM_ARRAY(arr)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(dtype)
        Z_PARAM_LONG(byte_offset)
    ZEND_PARSE_PARAMETERS_END();

    metal_buffer_t *intern = metal_buffer_from_obj(Z_OBJ_P(ZEND_THIS));
    HashTable *ht = Z_ARRVAL_P(arr);
    uint32_t count = zend_hash_num_elements(ht);

    size_t elem_size;
    switch (dtype) {
        case METAL_DTYPE_FLOAT:   elem_size = sizeof(float);    break;
        case METAL_DTYPE_DOUBLE:  elem_size = sizeof(double);   break;
        case METAL_DTYPE_INT32:   elem_size = sizeof(int32_t);  break;
        case METAL_DTYPE_UINT32:  elem_size = sizeof(uint32_t); break;
        case METAL_DTYPE_INT16:   elem_size = sizeof(int16_t);  break;
        case METAL_DTYPE_UINT16:  elem_size = sizeof(uint16_t); break;
        case METAL_DTYPE_INT8:    elem_size = sizeof(int8_t);   break;
        case METAL_DTYPE_UINT8:   elem_size = sizeof(uint8_t);  break;
        default:
            zend_throw_exception(metal_ce_exception, "Unknown data type", 0);
            RETURN_THROWS();
    }

    NSUInteger length = [intern->buffer length];
    if ((NSUInteger)byte_offset + count * elem_size > length) {
        zend_throw_exception(metal_ce_exception, "Write would exceed buffer bounds", 0);
        RETURN_THROWS();
    }

    void *contents = (char *)[intern->buffer contents] + byte_offset;
    uint32_t i = 0;
    zval *val;
    ZEND_HASH_FOREACH_VAL(ht, val) {
        switch (dtype) {
            case METAL_DTYPE_FLOAT:   ((float *)contents)[i]    = (float)zval_get_double(val);    break;
            case METAL_DTYPE_DOUBLE:  ((double *)contents)[i]   = zval_get_double(val);           break;
            case METAL_DTYPE_INT32:   ((int32_t *)contents)[i]  = (int32_t)zval_get_long(val);    break;
            case METAL_DTYPE_UINT32:  ((uint32_t *)contents)[i] = (uint32_t)zval_get_long(val);   break;
            case METAL_DTYPE_INT16:   ((int16_t *)contents)[i]  = (int16_t)zval_get_long(val);    break;
            case METAL_DTYPE_UINT16:  ((uint16_t *)contents)[i] = (uint16_t)zval_get_long(val);   break;
            case METAL_DTYPE_INT8:    ((int8_t *)contents)[i]   = (int8_t)zval_get_long(val);     break;
            case METAL_DTYPE_UINT8:   ((uint8_t *)contents)[i]  = (uint8_t)zval_get_long(val);    break;
        }
        i++;
    } ZEND_HASH_FOREACH_END();
}
/* }}} */

/* {{{ proto string Metal\Buffer::getRawContents(int $length = -1, int $offset = 0) */
PHP_METHOD(Metal_Buffer, getRawContents)
{
    zend_long length = -1;
    zend_long offset = 0;

    ZEND_PARSE_PARAMETERS_START(0, 2)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(length)
        Z_PARAM_LONG(offset)
    ZEND_PARSE_PARAMETERS_END();

    metal_buffer_t *intern = metal_buffer_from_obj(Z_OBJ_P(ZEND_THIS));
    NSUInteger buf_len = [intern->buffer length];

    if (length < 0) {
        length = (zend_long)(buf_len - (NSUInteger)offset);
    }

    if ((NSUInteger)offset + (NSUInteger)length > buf_len) {
        zend_throw_exception(metal_ce_exception, "Read would exceed buffer bounds", 0);
        RETURN_THROWS();
    }

    RETURN_STRINGL((char *)[intern->buffer contents] + offset, (size_t)length);
}
/* }}} */

/* {{{ proto void Metal\Buffer::writeRawContents(string $data, int $offset = 0) */
PHP_METHOD(Metal_Buffer, writeRawContents)
{
    zend_string *data;
    zend_long offset = 0;

    ZEND_PARSE_PARAMETERS_START(1, 2)
        Z_PARAM_STR(data)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(offset)
    ZEND_PARSE_PARAMETERS_END();

    metal_buffer_t *intern = metal_buffer_from_obj(Z_OBJ_P(ZEND_THIS));
    NSUInteger buf_len = [intern->buffer length];

    if ((NSUInteger)offset + ZSTR_LEN(data) > buf_len) {
        zend_throw_exception(metal_ce_exception, "Write would exceed buffer bounds", 0);
        RETURN_THROWS();
    }

    memcpy((char *)[intern->buffer contents] + offset, ZSTR_VAL(data), ZSTR_LEN(data));
}
/* }}} */

/* {{{ Enhanced Buffer methods */

PHP_METHOD(Metal_Buffer, didModifyRange)
{
    zend_long offset, length;
    ZEND_PARSE_PARAMETERS_START(2, 2) Z_PARAM_LONG(offset) Z_PARAM_LONG(length) ZEND_PARSE_PARAMETERS_END();
    metal_buffer_t *intern = metal_buffer_from_obj(Z_OBJ_P(ZEND_THIS));
    [intern->buffer didModifyRange:NSMakeRange((NSUInteger)offset, (NSUInteger)length)];
}

PHP_METHOD(Metal_Buffer, getGpuAddress)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_buffer_t *intern = metal_buffer_from_obj(Z_OBJ_P(ZEND_THIS));
    RETURN_LONG((zend_long)[intern->buffer gpuAddress]);
}

/* }}} */

/* ====================================================================
 *  Metal\CommandQueue methods
 * ==================================================================== */

/* {{{ proto Metal\CommandBuffer Metal\CommandQueue::createCommandBuffer() */
PHP_METHOD(Metal_CommandQueue, createCommandBuffer)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_command_queue_t *intern = metal_command_queue_from_obj(Z_OBJ_P(ZEND_THIS));
    id<MTLCommandBuffer> buf = [intern->queue commandBuffer];

    if (buf == nil) {
        zend_throw_exception(metal_ce_exception, "Failed to create command buffer", 0);
        RETURN_THROWS();
    }

    METAL_WRAP_RETURN(command_buffer, metal_command_buffer_t, buffer, metal_ce_command_buffer, buf);
}
/* }}} */

/* ====================================================================
 *  Metal\CommandBuffer methods
 * ==================================================================== */

/* {{{ proto Metal\ComputeCommandEncoder Metal\CommandBuffer::createComputeCommandEncoder() */
PHP_METHOD(Metal_CommandBuffer, createComputeCommandEncoder)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_command_buffer_t *intern = metal_command_buffer_from_obj(Z_OBJ_P(ZEND_THIS));
    id<MTLComputeCommandEncoder> enc = [intern->buffer computeCommandEncoder];

    if (enc == nil) {
        zend_throw_exception(metal_ce_exception, "Failed to create compute command encoder", 0);
        RETURN_THROWS();
    }

    METAL_WRAP_RETURN(compute_encoder, metal_compute_encoder_t, encoder, metal_ce_compute_encoder, enc);
}
/* }}} */

/* {{{ proto Metal\RenderCommandEncoder Metal\CommandBuffer::createRenderCommandEncoder(Metal\RenderPassDescriptor $descriptor) */
PHP_METHOD(Metal_CommandBuffer, createRenderCommandEncoder)
{
    zval *zdescriptor;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zdescriptor, metal_ce_render_pass_descriptor)
    ZEND_PARSE_PARAMETERS_END();

    metal_command_buffer_t *intern = metal_command_buffer_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_render_pass_descriptor_t *desc = metal_render_pass_descriptor_from_obj(Z_OBJ_P(zdescriptor));

    id<MTLRenderCommandEncoder> enc = [intern->buffer renderCommandEncoderWithDescriptor:desc->descriptor];
    if (enc == nil) {
        zend_throw_exception(metal_ce_exception, "Failed to create render command encoder", 0);
        RETURN_THROWS();
    }

    METAL_WRAP_RETURN(render_encoder, metal_render_encoder_t, encoder, metal_ce_render_encoder, enc);
}
/* }}} */

/* {{{ proto Metal\BlitCommandEncoder Metal\CommandBuffer::createBlitCommandEncoder() */
PHP_METHOD(Metal_CommandBuffer, createBlitCommandEncoder)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_command_buffer_t *intern = metal_command_buffer_from_obj(Z_OBJ_P(ZEND_THIS));
    id<MTLBlitCommandEncoder> enc = [intern->buffer blitCommandEncoder];

    if (enc == nil) {
        zend_throw_exception(metal_ce_exception, "Failed to create blit command encoder", 0);
        RETURN_THROWS();
    }

    METAL_WRAP_RETURN(blit_encoder, metal_blit_encoder_t, encoder, metal_ce_blit_encoder, enc);
}
/* }}} */

/* {{{ proto void Metal\CommandBuffer::commit() */
PHP_METHOD(Metal_CommandBuffer, commit)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_command_buffer_t *intern = metal_command_buffer_from_obj(Z_OBJ_P(ZEND_THIS));
    [intern->buffer commit];
}
/* }}} */

/* {{{ proto void Metal\CommandBuffer::waitUntilCompleted() */
PHP_METHOD(Metal_CommandBuffer, waitUntilCompleted)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_command_buffer_t *intern = metal_command_buffer_from_obj(Z_OBJ_P(ZEND_THIS));
    [intern->buffer waitUntilCompleted];

    /* Check for errors */
    NSError *error = [intern->buffer error];
    if (error != nil) {
        NSString *desc = [error localizedDescription];
        zend_throw_exception_ex(metal_ce_exception, 0,
            "Command buffer execution failed: %s", [desc UTF8String]);
        RETURN_THROWS();
    }
}
/* }}} */

/* {{{ proto void Metal\CommandBuffer::waitUntilScheduled() */
PHP_METHOD(Metal_CommandBuffer, waitUntilScheduled)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_command_buffer_t *intern = metal_command_buffer_from_obj(Z_OBJ_P(ZEND_THIS));
    [intern->buffer waitUntilScheduled];
}
/* }}} */

/* {{{ proto int Metal\CommandBuffer::getStatus() */
PHP_METHOD(Metal_CommandBuffer, getStatus)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_command_buffer_t *intern = metal_command_buffer_from_obj(Z_OBJ_P(ZEND_THIS));
    RETURN_LONG((zend_long)[intern->buffer status]);
}
/* }}} */

/* {{{ proto void Metal\CommandBuffer::presentDrawable(Metal\Drawable $drawable) */
PHP_METHOD(Metal_CommandBuffer, presentDrawable)
{
    zval *zdrawable;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zdrawable, metal_ce_drawable)
    ZEND_PARSE_PARAMETERS_END();

    metal_command_buffer_t *intern = metal_command_buffer_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_drawable_t *drw = metal_drawable_from_obj(Z_OBJ_P(zdrawable));

    [intern->buffer presentDrawable:drw->drawable];
}
/* }}} */

/* {{{ Enhanced CommandBuffer methods — event signaling */

PHP_METHOD(Metal_CommandBuffer, encodeSignalEvent)
{
    zval *zevent; zend_long value;
    ZEND_PARSE_PARAMETERS_START(2, 2) Z_PARAM_OBJECT(zevent) Z_PARAM_LONG(value) ZEND_PARSE_PARAMETERS_END();
    metal_command_buffer_t *intern = metal_command_buffer_from_obj(Z_OBJ_P(ZEND_THIS));
    if (instanceof_function(Z_OBJCE_P(zevent), metal_ce_shared_event)) {
        [intern->buffer encodeSignalEvent:metal_shared_event_from_obj(Z_OBJ_P(zevent))->event value:(uint64_t)value];
    } else if (instanceof_function(Z_OBJCE_P(zevent), metal_ce_event)) {
        [intern->buffer encodeSignalEvent:metal_event_from_obj(Z_OBJ_P(zevent))->event value:(uint64_t)value];
    } else {
        zend_throw_exception(metal_ce_exception, "Expected Event or SharedEvent", 0); RETURN_THROWS();
    }
}

PHP_METHOD(Metal_CommandBuffer, encodeWaitForEvent)
{
    zval *zevent; zend_long value;
    ZEND_PARSE_PARAMETERS_START(2, 2) Z_PARAM_OBJECT(zevent) Z_PARAM_LONG(value) ZEND_PARSE_PARAMETERS_END();
    metal_command_buffer_t *intern = metal_command_buffer_from_obj(Z_OBJ_P(ZEND_THIS));
    if (instanceof_function(Z_OBJCE_P(zevent), metal_ce_shared_event)) {
        [intern->buffer encodeWaitForEvent:metal_shared_event_from_obj(Z_OBJ_P(zevent))->event value:(uint64_t)value];
    } else if (instanceof_function(Z_OBJCE_P(zevent), metal_ce_event)) {
        [intern->buffer encodeWaitForEvent:metal_event_from_obj(Z_OBJ_P(zevent))->event value:(uint64_t)value];
    } else {
        zend_throw_exception(metal_ce_exception, "Expected Event or SharedEvent", 0); RETURN_THROWS();
    }
}

PHP_METHOD(Metal_CommandBuffer, createComputeCommandEncoderWithDescriptor)
{
    zval *zdesc;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_OBJECT_OF_CLASS(zdesc, metal_ce_compute_pass_descriptor) ZEND_PARSE_PARAMETERS_END();
    metal_command_buffer_t *intern = metal_command_buffer_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_compute_pass_descriptor_t *desc = metal_compute_pass_descriptor_from_obj(Z_OBJ_P(zdesc));
    id<MTLComputeCommandEncoder> enc = [intern->buffer computeCommandEncoderWithDescriptor:desc->descriptor];
    if (!enc) { zend_throw_exception(metal_ce_exception, "Failed to create compute encoder with descriptor", 0); RETURN_THROWS(); }
    METAL_WRAP_RETURN(compute_encoder, metal_compute_encoder_t, encoder, metal_ce_compute_encoder, enc);
}

PHP_METHOD(Metal_CommandBuffer, createAccelerationStructureCommandEncoder)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_command_buffer_t *intern = metal_command_buffer_from_obj(Z_OBJ_P(ZEND_THIS));
    id<MTLAccelerationStructureCommandEncoder> enc = [intern->buffer accelerationStructureCommandEncoder];
    if (!enc) { zend_throw_exception(metal_ce_exception, "Failed to create acceleration structure command encoder", 0); RETURN_THROWS(); }
    METAL_WRAP_RETURN(accel_encoder, metal_accel_encoder_t, encoder, metal_ce_accel_encoder, enc);
}

/* }}} */

/* ====================================================================
 *  Metal\ComputeCommandEncoder methods
 * ==================================================================== */

/* {{{ proto void Metal\ComputeCommandEncoder::setComputePipelineState(Metal\ComputePipelineState $state) */
PHP_METHOD(Metal_ComputeCommandEncoder, setComputePipelineState)
{
    zval *zstate;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zstate, metal_ce_compute_pipeline_state)
    ZEND_PARSE_PARAMETERS_END();

    metal_compute_encoder_t *intern = metal_compute_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_compute_pipeline_state_t *pso = metal_compute_pipeline_state_from_obj(Z_OBJ_P(zstate));

    [intern->encoder setComputePipelineState:pso->state];
}
/* }}} */

/* {{{ proto void Metal\ComputeCommandEncoder::setBuffer(Metal\Buffer $buffer, int $offset, int $index) */
PHP_METHOD(Metal_ComputeCommandEncoder, setBuffer)
{
    zval *zbuffer;
    zend_long offset, index;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_OBJECT_OF_CLASS(zbuffer, metal_ce_buffer)
        Z_PARAM_LONG(offset)
        Z_PARAM_LONG(index)
    ZEND_PARSE_PARAMETERS_END();

    metal_compute_encoder_t *intern = metal_compute_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_buffer_t *buf = metal_buffer_from_obj(Z_OBJ_P(zbuffer));

    [intern->encoder setBuffer:buf->buffer offset:(NSUInteger)offset atIndex:(NSUInteger)index];
}
/* }}} */

/* {{{ proto void Metal\ComputeCommandEncoder::setBytes(string $data, int $index) */
PHP_METHOD(Metal_ComputeCommandEncoder, setBytes)
{
    zend_string *data;
    zend_long index;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STR(data)
        Z_PARAM_LONG(index)
    ZEND_PARSE_PARAMETERS_END();

    metal_compute_encoder_t *intern = metal_compute_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    [intern->encoder setBytes:ZSTR_VAL(data) length:ZSTR_LEN(data) atIndex:(NSUInteger)index];
}
/* }}} */

/* {{{ proto void Metal\ComputeCommandEncoder::setTexture(Metal\Texture $texture, int $index) */
PHP_METHOD(Metal_ComputeCommandEncoder, setTexture)
{
    zval *ztexture;
    zend_long index;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_OBJECT_OF_CLASS(ztexture, metal_ce_texture)
        Z_PARAM_LONG(index)
    ZEND_PARSE_PARAMETERS_END();

    metal_compute_encoder_t *intern = metal_compute_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_texture_t *tex = metal_texture_from_obj(Z_OBJ_P(ztexture));

    [intern->encoder setTexture:tex->texture atIndex:(NSUInteger)index];
}
/* }}} */

/* {{{ proto void Metal\ComputeCommandEncoder::setSamplerState(Metal\SamplerState $sampler, int $index) */
PHP_METHOD(Metal_ComputeCommandEncoder, setSamplerState)
{
    zval *zsampler;
    zend_long index;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_OBJECT_OF_CLASS(zsampler, metal_ce_sampler_state)
        Z_PARAM_LONG(index)
    ZEND_PARSE_PARAMETERS_END();

    metal_compute_encoder_t *intern = metal_compute_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_sampler_state_t *smp = metal_sampler_state_from_obj(Z_OBJ_P(zsampler));

    [intern->encoder setSamplerState:smp->state atIndex:(NSUInteger)index];
}
/* }}} */

/* {{{ proto void Metal\ComputeCommandEncoder::setThreadgroupMemoryLength(int $length, int $index) */
PHP_METHOD(Metal_ComputeCommandEncoder, setThreadgroupMemoryLength)
{
    zend_long length, index;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_LONG(length)
        Z_PARAM_LONG(index)
    ZEND_PARSE_PARAMETERS_END();

    metal_compute_encoder_t *intern = metal_compute_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    [intern->encoder setThreadgroupMemoryLength:(NSUInteger)length atIndex:(NSUInteger)index];
}
/* }}} */

/* metal_parse_size is in metal_internal.h */

/* {{{ proto void Metal\ComputeCommandEncoder::dispatchThreads(array $gridSize, array $threadgroupSize) */
PHP_METHOD(Metal_ComputeCommandEncoder, dispatchThreads)
{
    zval *grid_arr, *tg_arr;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_ARRAY(grid_arr)
        Z_PARAM_ARRAY(tg_arr)
    ZEND_PARSE_PARAMETERS_END();

    MTLSize grid = metal_parse_size(grid_arr);
    MTLSize tg   = metal_parse_size(tg_arr);

    metal_compute_encoder_t *intern = metal_compute_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    [intern->encoder dispatchThreads:grid threadsPerThreadgroup:tg];
}
/* }}} */

/* {{{ proto void Metal\ComputeCommandEncoder::dispatchThreadgroups(array $threadgroupsPerGrid, array $threadsPerThreadgroup) */
PHP_METHOD(Metal_ComputeCommandEncoder, dispatchThreadgroups)
{
    zval *tgpg_arr, *tptg_arr;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_ARRAY(tgpg_arr)
        Z_PARAM_ARRAY(tptg_arr)
    ZEND_PARSE_PARAMETERS_END();

    MTLSize tgpg = metal_parse_size(tgpg_arr);
    MTLSize tptg = metal_parse_size(tptg_arr);

    metal_compute_encoder_t *intern = metal_compute_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    [intern->encoder dispatchThreadgroups:tgpg threadsPerThreadgroup:tptg];
}
/* }}} */

/* {{{ proto void Metal\ComputeCommandEncoder::endEncoding() */
PHP_METHOD(Metal_ComputeCommandEncoder, endEncoding)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_compute_encoder_t *intern = metal_compute_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    [intern->encoder endEncoding];
}
/* }}} */

/* {{{ Enhanced ComputeCommandEncoder methods */

PHP_METHOD(Metal_ComputeCommandEncoder, dispatchThreadsWithIndirectBuffer)
{
    zval *zbuf; zend_long offset; zval *tg_arr;
    ZEND_PARSE_PARAMETERS_START(3, 3) Z_PARAM_OBJECT_OF_CLASS(zbuf, metal_ce_buffer) Z_PARAM_LONG(offset) Z_PARAM_ARRAY(tg_arr) ZEND_PARSE_PARAMETERS_END();
    metal_compute_encoder_t *intern = metal_compute_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_buffer_t *buf = metal_buffer_from_obj(Z_OBJ_P(zbuf));
    MTLSize tg = metal_parse_size(tg_arr);
    [intern->encoder dispatchThreadgroupsWithIndirectBuffer:buf->buffer indirectBufferOffset:(NSUInteger)offset threadsPerThreadgroup:tg];
}

PHP_METHOD(Metal_ComputeCommandEncoder, useResource)
{
    zval *zres; zend_long usage;
    ZEND_PARSE_PARAMETERS_START(2, 2) Z_PARAM_OBJECT_OF_CLASS(zres, metal_ce_buffer) Z_PARAM_LONG(usage) ZEND_PARSE_PARAMETERS_END();
    metal_compute_encoder_t *intern = metal_compute_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_buffer_t *buf = metal_buffer_from_obj(Z_OBJ_P(zres));
    [intern->encoder useResource:buf->buffer usage:(MTLResourceUsage)usage];
}

PHP_METHOD(Metal_ComputeCommandEncoder, memoryBarrierWithScope)
{
    zend_long scope;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(scope) ZEND_PARSE_PARAMETERS_END();
    metal_compute_encoder_t *intern = metal_compute_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    [intern->encoder memoryBarrierWithScope:(MTLBarrierScope)scope];
}

PHP_METHOD(Metal_ComputeCommandEncoder, updateFence)
{
    zval *zfence;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_OBJECT_OF_CLASS(zfence, metal_ce_fence) ZEND_PARSE_PARAMETERS_END();
    [metal_compute_encoder_from_obj(Z_OBJ_P(ZEND_THIS))->encoder updateFence:metal_fence_from_obj(Z_OBJ_P(zfence))->fence];
}

PHP_METHOD(Metal_ComputeCommandEncoder, waitForFence)
{
    zval *zfence;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_OBJECT_OF_CLASS(zfence, metal_ce_fence) ZEND_PARSE_PARAMETERS_END();
    [metal_compute_encoder_from_obj(Z_OBJ_P(ZEND_THIS))->encoder waitForFence:metal_fence_from_obj(Z_OBJ_P(zfence))->fence];
}

PHP_METHOD(Metal_ComputeCommandEncoder, setAccelerationStructure)
{
    zval *zas; zend_long index;
    ZEND_PARSE_PARAMETERS_START(2, 2) Z_PARAM_OBJECT_OF_CLASS(zas, metal_ce_acceleration_structure) Z_PARAM_LONG(index) ZEND_PARSE_PARAMETERS_END();
    [metal_compute_encoder_from_obj(Z_OBJ_P(ZEND_THIS))->encoder setAccelerationStructure:metal_acceleration_structure_from_obj(Z_OBJ_P(zas))->accel atBufferIndex:(NSUInteger)index];
}

/* }}} */

/* ====================================================================
 *  Metal\RenderCommandEncoder methods
 * ==================================================================== */

/* {{{ proto void Metal\RenderCommandEncoder::setRenderPipelineState(Metal\RenderPipelineState $state) */
PHP_METHOD(Metal_RenderCommandEncoder, setRenderPipelineState)
{
    zval *zstate;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zstate, metal_ce_render_pipeline_state)
    ZEND_PARSE_PARAMETERS_END();

    metal_render_encoder_t *intern = metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_render_pipeline_state_t *pso = metal_render_pipeline_state_from_obj(Z_OBJ_P(zstate));

    [intern->encoder setRenderPipelineState:pso->state];
}
/* }}} */

/* {{{ proto void Metal\RenderCommandEncoder::setVertexBuffer(Metal\Buffer $buffer, int $offset, int $index) */
PHP_METHOD(Metal_RenderCommandEncoder, setVertexBuffer)
{
    zval *zbuffer;
    zend_long offset, index;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_OBJECT_OF_CLASS(zbuffer, metal_ce_buffer)
        Z_PARAM_LONG(offset)
        Z_PARAM_LONG(index)
    ZEND_PARSE_PARAMETERS_END();

    metal_render_encoder_t *intern = metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_buffer_t *buf = metal_buffer_from_obj(Z_OBJ_P(zbuffer));

    [intern->encoder setVertexBuffer:buf->buffer offset:(NSUInteger)offset atIndex:(NSUInteger)index];
}
/* }}} */

/* {{{ proto void Metal\RenderCommandEncoder::setVertexBytes(string $data, int $index) */
PHP_METHOD(Metal_RenderCommandEncoder, setVertexBytes)
{
    zend_string *data;
    zend_long index;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STR(data)
        Z_PARAM_LONG(index)
    ZEND_PARSE_PARAMETERS_END();

    metal_render_encoder_t *intern = metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    [intern->encoder setVertexBytes:ZSTR_VAL(data) length:ZSTR_LEN(data) atIndex:(NSUInteger)index];
}
/* }}} */

/* {{{ proto void Metal\RenderCommandEncoder::setFragmentBuffer(Metal\Buffer $buffer, int $offset, int $index) */
PHP_METHOD(Metal_RenderCommandEncoder, setFragmentBuffer)
{
    zval *zbuffer;
    zend_long offset, index;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_OBJECT_OF_CLASS(zbuffer, metal_ce_buffer)
        Z_PARAM_LONG(offset)
        Z_PARAM_LONG(index)
    ZEND_PARSE_PARAMETERS_END();

    metal_render_encoder_t *intern = metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_buffer_t *buf = metal_buffer_from_obj(Z_OBJ_P(zbuffer));

    [intern->encoder setFragmentBuffer:buf->buffer offset:(NSUInteger)offset atIndex:(NSUInteger)index];
}
/* }}} */

/* {{{ proto void Metal\RenderCommandEncoder::setFragmentBytes(string $data, int $index) */
PHP_METHOD(Metal_RenderCommandEncoder, setFragmentBytes)
{
    zend_string *data;
    zend_long index;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STR(data)
        Z_PARAM_LONG(index)
    ZEND_PARSE_PARAMETERS_END();

    metal_render_encoder_t *intern = metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    [intern->encoder setFragmentBytes:ZSTR_VAL(data) length:ZSTR_LEN(data) atIndex:(NSUInteger)index];
}
/* }}} */

/* {{{ proto void Metal\RenderCommandEncoder::setFragmentTexture(Metal\Texture $texture, int $index) */
PHP_METHOD(Metal_RenderCommandEncoder, setFragmentTexture)
{
    zval *ztexture;
    zend_long index;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_OBJECT_OF_CLASS(ztexture, metal_ce_texture)
        Z_PARAM_LONG(index)
    ZEND_PARSE_PARAMETERS_END();

    metal_render_encoder_t *intern = metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_texture_t *tex = metal_texture_from_obj(Z_OBJ_P(ztexture));

    [intern->encoder setFragmentTexture:tex->texture atIndex:(NSUInteger)index];
}
/* }}} */

/* {{{ proto void Metal\RenderCommandEncoder::setFragmentSamplerState(Metal\SamplerState $sampler, int $index) */
PHP_METHOD(Metal_RenderCommandEncoder, setFragmentSamplerState)
{
    zval *zsampler;
    zend_long index;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_OBJECT_OF_CLASS(zsampler, metal_ce_sampler_state)
        Z_PARAM_LONG(index)
    ZEND_PARSE_PARAMETERS_END();

    metal_render_encoder_t *intern = metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_sampler_state_t *smp = metal_sampler_state_from_obj(Z_OBJ_P(zsampler));

    [intern->encoder setFragmentSamplerState:smp->state atIndex:(NSUInteger)index];
}
/* }}} */

/* {{{ proto void Metal\RenderCommandEncoder::setDepthStencilState(Metal\DepthStencilState $state) */
PHP_METHOD(Metal_RenderCommandEncoder, setDepthStencilState)
{
    zval *zstate;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zstate, metal_ce_depth_stencil_state)
    ZEND_PARSE_PARAMETERS_END();

    metal_render_encoder_t *intern = metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_depth_stencil_state_t *dss = metal_depth_stencil_state_from_obj(Z_OBJ_P(zstate));

    [intern->encoder setDepthStencilState:dss->state];
}
/* }}} */

/* {{{ proto void Metal\RenderCommandEncoder::setViewport(float $x, float $y, float $width, float $height, float $znear, float $zfar) */
PHP_METHOD(Metal_RenderCommandEncoder, setViewport)
{
    double x, y, w, h, znear, zfar;

    ZEND_PARSE_PARAMETERS_START(6, 6)
        Z_PARAM_DOUBLE(x)
        Z_PARAM_DOUBLE(y)
        Z_PARAM_DOUBLE(w)
        Z_PARAM_DOUBLE(h)
        Z_PARAM_DOUBLE(znear)
        Z_PARAM_DOUBLE(zfar)
    ZEND_PARSE_PARAMETERS_END();

    metal_render_encoder_t *intern = metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    MTLViewport viewport = { x, y, w, h, znear, zfar };
    [intern->encoder setViewport:viewport];
}
/* }}} */

/* {{{ proto void Metal\RenderCommandEncoder::setScissorRect(int $x, int $y, int $width, int $height) */
PHP_METHOD(Metal_RenderCommandEncoder, setScissorRect)
{
    zend_long x, y, w, h;

    ZEND_PARSE_PARAMETERS_START(4, 4)
        Z_PARAM_LONG(x)
        Z_PARAM_LONG(y)
        Z_PARAM_LONG(w)
        Z_PARAM_LONG(h)
    ZEND_PARSE_PARAMETERS_END();

    metal_render_encoder_t *intern = metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    MTLScissorRect rect = { (NSUInteger)x, (NSUInteger)y, (NSUInteger)w, (NSUInteger)h };
    [intern->encoder setScissorRect:rect];
}
/* }}} */

/* {{{ proto void Metal\RenderCommandEncoder::setCullMode(int $mode) */
PHP_METHOD(Metal_RenderCommandEncoder, setCullMode)
{
    zend_long mode;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(mode)
    ZEND_PARSE_PARAMETERS_END();

    metal_render_encoder_t *intern = metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    [intern->encoder setCullMode:(MTLCullMode)mode];
}
/* }}} */

/* {{{ proto void Metal\RenderCommandEncoder::setFrontFacingWinding(int $winding) */
PHP_METHOD(Metal_RenderCommandEncoder, setFrontFacingWinding)
{
    zend_long winding;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(winding)
    ZEND_PARSE_PARAMETERS_END();

    metal_render_encoder_t *intern = metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    [intern->encoder setFrontFacingWinding:(MTLWinding)winding];
}
/* }}} */

/* {{{ proto void Metal\RenderCommandEncoder::setTriangleFillMode(int $mode) */
PHP_METHOD(Metal_RenderCommandEncoder, setTriangleFillMode)
{
    zend_long mode;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(mode)
    ZEND_PARSE_PARAMETERS_END();

    metal_render_encoder_t *intern = metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    [intern->encoder setTriangleFillMode:(MTLTriangleFillMode)mode];
}
/* }}} */

/* {{{ proto void Metal\RenderCommandEncoder::drawPrimitives(int $type, int $vertexStart, int $vertexCount) */
PHP_METHOD(Metal_RenderCommandEncoder, drawPrimitives)
{
    zend_long type, start, count;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_LONG(type)
        Z_PARAM_LONG(start)
        Z_PARAM_LONG(count)
    ZEND_PARSE_PARAMETERS_END();

    metal_render_encoder_t *intern = metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    [intern->encoder drawPrimitives:(MTLPrimitiveType)type
                        vertexStart:(NSUInteger)start
                        vertexCount:(NSUInteger)count];
}
/* }}} */

/* {{{ proto void Metal\RenderCommandEncoder::drawIndexedPrimitives(int $type, int $indexCount, int $indexType, Metal\Buffer $indexBuffer, int $indexBufferOffset) */
PHP_METHOD(Metal_RenderCommandEncoder, drawIndexedPrimitives)
{
    zend_long type, index_count, index_type, index_buffer_offset;
    zval *zindex_buffer;

    ZEND_PARSE_PARAMETERS_START(5, 5)
        Z_PARAM_LONG(type)
        Z_PARAM_LONG(index_count)
        Z_PARAM_LONG(index_type)
        Z_PARAM_OBJECT_OF_CLASS(zindex_buffer, metal_ce_buffer)
        Z_PARAM_LONG(index_buffer_offset)
    ZEND_PARSE_PARAMETERS_END();

    metal_render_encoder_t *intern = metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_buffer_t *ibuf = metal_buffer_from_obj(Z_OBJ_P(zindex_buffer));

    [intern->encoder drawIndexedPrimitives:(MTLPrimitiveType)type
                                indexCount:(NSUInteger)index_count
                                 indexType:(MTLIndexType)index_type
                               indexBuffer:ibuf->buffer
                         indexBufferOffset:(NSUInteger)index_buffer_offset];
}
/* }}} */

/* {{{ proto void Metal\RenderCommandEncoder::endEncoding() */
PHP_METHOD(Metal_RenderCommandEncoder, endEncoding)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_render_encoder_t *intern = metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    [intern->encoder endEncoding];
}
/* }}} */

/* {{{ Enhanced RenderCommandEncoder methods */

PHP_METHOD(Metal_RenderCommandEncoder, setVertexTexture)
{
    zval *ztex; zend_long index;
    ZEND_PARSE_PARAMETERS_START(2, 2) Z_PARAM_OBJECT_OF_CLASS(ztex, metal_ce_texture) Z_PARAM_LONG(index) ZEND_PARSE_PARAMETERS_END();
    [metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS))->encoder setVertexTexture:metal_texture_from_obj(Z_OBJ_P(ztex))->texture atIndex:(NSUInteger)index];
}

PHP_METHOD(Metal_RenderCommandEncoder, setVertexSamplerState)
{
    zval *zsmp; zend_long index;
    ZEND_PARSE_PARAMETERS_START(2, 2) Z_PARAM_OBJECT_OF_CLASS(zsmp, metal_ce_sampler_state) Z_PARAM_LONG(index) ZEND_PARSE_PARAMETERS_END();
    [metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS))->encoder setVertexSamplerState:metal_sampler_state_from_obj(Z_OBJ_P(zsmp))->state atIndex:(NSUInteger)index];
}

PHP_METHOD(Metal_RenderCommandEncoder, drawPrimitivesInstanced)
{
    zend_long type, start, count, instances;
    ZEND_PARSE_PARAMETERS_START(4, 4) Z_PARAM_LONG(type) Z_PARAM_LONG(start) Z_PARAM_LONG(count) Z_PARAM_LONG(instances) ZEND_PARSE_PARAMETERS_END();
    [metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS))->encoder drawPrimitives:(MTLPrimitiveType)type vertexStart:(NSUInteger)start vertexCount:(NSUInteger)count instanceCount:(NSUInteger)instances];
}

PHP_METHOD(Metal_RenderCommandEncoder, drawIndexedPrimitivesInstanced)
{
    zend_long type, index_count, index_type, ibo_offset, instances;
    zval *zibo;
    ZEND_PARSE_PARAMETERS_START(6, 6) Z_PARAM_LONG(type) Z_PARAM_LONG(index_count) Z_PARAM_LONG(index_type) Z_PARAM_OBJECT_OF_CLASS(zibo, metal_ce_buffer) Z_PARAM_LONG(ibo_offset) Z_PARAM_LONG(instances) ZEND_PARSE_PARAMETERS_END();
    [metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS))->encoder drawIndexedPrimitives:(MTLPrimitiveType)type indexCount:(NSUInteger)index_count indexType:(MTLIndexType)index_type indexBuffer:metal_buffer_from_obj(Z_OBJ_P(zibo))->buffer indexBufferOffset:(NSUInteger)ibo_offset instanceCount:(NSUInteger)instances];
}

PHP_METHOD(Metal_RenderCommandEncoder, drawPrimitivesIndirect)
{
    zend_long type; zval *zbuf; zend_long offset;
    ZEND_PARSE_PARAMETERS_START(3, 3) Z_PARAM_LONG(type) Z_PARAM_OBJECT_OF_CLASS(zbuf, metal_ce_buffer) Z_PARAM_LONG(offset) ZEND_PARSE_PARAMETERS_END();
    [metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS))->encoder drawPrimitives:(MTLPrimitiveType)type indirectBuffer:metal_buffer_from_obj(Z_OBJ_P(zbuf))->buffer indirectBufferOffset:(NSUInteger)offset];
}

PHP_METHOD(Metal_RenderCommandEncoder, drawIndexedPrimitivesIndirect)
{
    zend_long type, index_type, ibo_offset; zval *zibo, *zbuf; zend_long offset;
    ZEND_PARSE_PARAMETERS_START(6, 6) Z_PARAM_LONG(type) Z_PARAM_LONG(index_type) Z_PARAM_OBJECT_OF_CLASS(zibo, metal_ce_buffer) Z_PARAM_LONG(ibo_offset) Z_PARAM_OBJECT_OF_CLASS(zbuf, metal_ce_buffer) Z_PARAM_LONG(offset) ZEND_PARSE_PARAMETERS_END();
    [metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS))->encoder drawIndexedPrimitives:(MTLPrimitiveType)type indexType:(MTLIndexType)index_type indexBuffer:metal_buffer_from_obj(Z_OBJ_P(zibo))->buffer indexBufferOffset:(NSUInteger)ibo_offset indirectBuffer:metal_buffer_from_obj(Z_OBJ_P(zbuf))->buffer indirectBufferOffset:(NSUInteger)offset];
}

PHP_METHOD(Metal_RenderCommandEncoder, setBlendColor)
{
    double r, g, b, a;
    ZEND_PARSE_PARAMETERS_START(4, 4) Z_PARAM_DOUBLE(r) Z_PARAM_DOUBLE(g) Z_PARAM_DOUBLE(b) Z_PARAM_DOUBLE(a) ZEND_PARSE_PARAMETERS_END();
    [metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS))->encoder setBlendColorRed:r green:g blue:b alpha:a];
}

PHP_METHOD(Metal_RenderCommandEncoder, setStencilReferenceValue)
{
    zend_long value;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(value) ZEND_PARSE_PARAMETERS_END();
    [metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS))->encoder setStencilReferenceValue:(uint32_t)value];
}

PHP_METHOD(Metal_RenderCommandEncoder, setDepthBias)
{
    double bias, slope, clamp;
    ZEND_PARSE_PARAMETERS_START(3, 3) Z_PARAM_DOUBLE(bias) Z_PARAM_DOUBLE(slope) Z_PARAM_DOUBLE(clamp) ZEND_PARSE_PARAMETERS_END();
    [metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS))->encoder setDepthBias:bias slopeScale:slope clamp:clamp];
}

PHP_METHOD(Metal_RenderCommandEncoder, updateFence)
{
    zval *zfence; zend_long stages;
    ZEND_PARSE_PARAMETERS_START(2, 2) Z_PARAM_OBJECT_OF_CLASS(zfence, metal_ce_fence) Z_PARAM_LONG(stages) ZEND_PARSE_PARAMETERS_END();
    [metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS))->encoder updateFence:metal_fence_from_obj(Z_OBJ_P(zfence))->fence afterStages:(MTLRenderStages)stages];
}

PHP_METHOD(Metal_RenderCommandEncoder, waitForFence)
{
    zval *zfence; zend_long stages;
    ZEND_PARSE_PARAMETERS_START(2, 2) Z_PARAM_OBJECT_OF_CLASS(zfence, metal_ce_fence) Z_PARAM_LONG(stages) ZEND_PARSE_PARAMETERS_END();
    [metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS))->encoder waitForFence:metal_fence_from_obj(Z_OBJ_P(zfence))->fence beforeStages:(MTLRenderStages)stages];
}

PHP_METHOD(Metal_RenderCommandEncoder, useResource)
{
    zval *zres; zend_long usage, stages;
    ZEND_PARSE_PARAMETERS_START(3, 3) Z_PARAM_OBJECT_OF_CLASS(zres, metal_ce_buffer) Z_PARAM_LONG(usage) Z_PARAM_LONG(stages) ZEND_PARSE_PARAMETERS_END();
    [metal_render_encoder_from_obj(Z_OBJ_P(ZEND_THIS))->encoder useResource:metal_buffer_from_obj(Z_OBJ_P(zres))->buffer usage:(MTLResourceUsage)usage stages:(MTLRenderStages)stages];
}

/* }}} */

/* ====================================================================
 *  Metal\BlitCommandEncoder methods
 * ==================================================================== */

/* {{{ proto void Metal\BlitCommandEncoder::copyFromBuffer(Metal\Buffer $src, int $srcOffset, Metal\Buffer $dst, int $dstOffset, int $size) */
PHP_METHOD(Metal_BlitCommandEncoder, copyFromBuffer)
{
    zval *zsrc, *zdst;
    zend_long src_offset, dst_offset, size;

    ZEND_PARSE_PARAMETERS_START(5, 5)
        Z_PARAM_OBJECT_OF_CLASS(zsrc, metal_ce_buffer)
        Z_PARAM_LONG(src_offset)
        Z_PARAM_OBJECT_OF_CLASS(zdst, metal_ce_buffer)
        Z_PARAM_LONG(dst_offset)
        Z_PARAM_LONG(size)
    ZEND_PARSE_PARAMETERS_END();

    metal_blit_encoder_t *intern = metal_blit_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_buffer_t *src = metal_buffer_from_obj(Z_OBJ_P(zsrc));
    metal_buffer_t *dst = metal_buffer_from_obj(Z_OBJ_P(zdst));

    [intern->encoder copyFromBuffer:src->buffer
                       sourceOffset:(NSUInteger)src_offset
                           toBuffer:dst->buffer
                  destinationOffset:(NSUInteger)dst_offset
                               size:(NSUInteger)size];
}
/* }}} */

/* {{{ proto void Metal\BlitCommandEncoder::synchronizeResource(Metal\Buffer $buffer) */
PHP_METHOD(Metal_BlitCommandEncoder, synchronizeResource)
{
    zval *zbuffer;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zbuffer, metal_ce_buffer)
    ZEND_PARSE_PARAMETERS_END();

    metal_blit_encoder_t *intern = metal_blit_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_buffer_t *buf = metal_buffer_from_obj(Z_OBJ_P(zbuffer));

    [intern->encoder synchronizeResource:buf->buffer];
}
/* }}} */

/* {{{ proto void Metal\BlitCommandEncoder::endEncoding() */
PHP_METHOD(Metal_BlitCommandEncoder, endEncoding)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_blit_encoder_t *intern = metal_blit_encoder_from_obj(Z_OBJ_P(ZEND_THIS));
    [intern->encoder endEncoding];
}
/* }}} */

/* {{{ Enhanced BlitCommandEncoder methods */

PHP_METHOD(Metal_BlitCommandEncoder, generateMipmaps)
{
    zval *ztex;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_OBJECT_OF_CLASS(ztex, metal_ce_texture) ZEND_PARSE_PARAMETERS_END();
    [metal_blit_encoder_from_obj(Z_OBJ_P(ZEND_THIS))->encoder generateMipmapsForTexture:metal_texture_from_obj(Z_OBJ_P(ztex))->texture];
}

PHP_METHOD(Metal_BlitCommandEncoder, fillBuffer)
{
    zval *zbuf; zend_long offset, length, value;
    ZEND_PARSE_PARAMETERS_START(4, 4) Z_PARAM_OBJECT_OF_CLASS(zbuf, metal_ce_buffer) Z_PARAM_LONG(offset) Z_PARAM_LONG(length) Z_PARAM_LONG(value) ZEND_PARSE_PARAMETERS_END();
    [metal_blit_encoder_from_obj(Z_OBJ_P(ZEND_THIS))->encoder fillBuffer:metal_buffer_from_obj(Z_OBJ_P(zbuf))->buffer range:NSMakeRange((NSUInteger)offset, (NSUInteger)length) value:(uint8_t)value];
}

PHP_METHOD(Metal_BlitCommandEncoder, copyFromTexture)
{
    zval *zsrc, *zdst;
    zend_long src_slice, src_level, dst_slice, dst_level;
    zval *src_origin_arr, *src_size_arr, *dst_origin_arr;

    ZEND_PARSE_PARAMETERS_START(8, 8)
        Z_PARAM_OBJECT_OF_CLASS(zsrc, metal_ce_texture) Z_PARAM_LONG(src_slice) Z_PARAM_LONG(src_level)
        Z_PARAM_ARRAY(src_origin_arr) Z_PARAM_ARRAY(src_size_arr)
        Z_PARAM_OBJECT_OF_CLASS(zdst, metal_ce_texture) Z_PARAM_LONG(dst_slice) Z_PARAM_LONG(dst_level)
    ZEND_PARSE_PARAMETERS_END();

    /* Simplified: use dst_origin = src_origin for now. A proper 9-arg version can be added later. */
    MTLOrigin src_origin = {0, 0, 0};
    MTLSize src_size = metal_parse_size(src_size_arr);
    MTLOrigin dst_origin = {0, 0, 0};

    HashTable *ht = Z_ARRVAL_P(src_origin_arr);
    zval *v;
    if ((v = zend_hash_index_find(ht, 0))) src_origin.x = (NSUInteger)zval_get_long(v);
    if ((v = zend_hash_index_find(ht, 1))) src_origin.y = (NSUInteger)zval_get_long(v);
    if ((v = zend_hash_index_find(ht, 2))) src_origin.z = (NSUInteger)zval_get_long(v);
    dst_origin = src_origin; /* default: same position */

    [metal_blit_encoder_from_obj(Z_OBJ_P(ZEND_THIS))->encoder
        copyFromTexture:metal_texture_from_obj(Z_OBJ_P(zsrc))->texture sourceSlice:(NSUInteger)src_slice sourceLevel:(NSUInteger)src_level sourceOrigin:src_origin sourceSize:src_size
        toTexture:metal_texture_from_obj(Z_OBJ_P(zdst))->texture destinationSlice:(NSUInteger)dst_slice destinationLevel:(NSUInteger)dst_level destinationOrigin:dst_origin];
}

PHP_METHOD(Metal_BlitCommandEncoder, optimizeContentsForGPUAccess)
{
    zval *ztex;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_OBJECT_OF_CLASS(ztex, metal_ce_texture) ZEND_PARSE_PARAMETERS_END();
    [metal_blit_encoder_from_obj(Z_OBJ_P(ZEND_THIS))->encoder optimizeContentsForGPUAccess:metal_texture_from_obj(Z_OBJ_P(ztex))->texture];
}

PHP_METHOD(Metal_BlitCommandEncoder, optimizeContentsForCPUAccess)
{
    zval *ztex;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_OBJECT_OF_CLASS(ztex, metal_ce_texture) ZEND_PARSE_PARAMETERS_END();
    [metal_blit_encoder_from_obj(Z_OBJ_P(ZEND_THIS))->encoder optimizeContentsForCPUAccess:metal_texture_from_obj(Z_OBJ_P(ztex))->texture];
}

/* Note: buildAccelerationStructure requires MTLAccelerationStructureCommandEncoder,
   which is obtained via [commandBuffer accelerationStructureCommandEncoder].
   This will be added in a future version with a dedicated AccelerationStructureCommandEncoder class. */

/* }}} */

/* {{{ Enhanced Texture methods */

PHP_METHOD(Metal_Texture, newTextureView)
{
    zend_long format;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(format) ZEND_PARSE_PARAMETERS_END();
    metal_texture_t *intern = metal_texture_from_obj(Z_OBJ_P(ZEND_THIS));
    id<MTLTexture> view = [intern->texture newTextureViewWithPixelFormat:(MTLPixelFormat)format];
    if (!view) { zend_throw_exception(metal_ce_exception, "Failed to create texture view", 0); RETURN_THROWS(); }
    METAL_WRAP_RETURN(texture, metal_texture_t, texture, metal_ce_texture, view);
}

PHP_METHOD(Metal_Texture, getUsage) { ZEND_PARSE_PARAMETERS_NONE(); RETURN_LONG((zend_long)[metal_texture_from_obj(Z_OBJ_P(ZEND_THIS))->texture usage]); }
PHP_METHOD(Metal_Texture, getStorageMode) { ZEND_PARSE_PARAMETERS_NONE(); RETURN_LONG((zend_long)[metal_texture_from_obj(Z_OBJ_P(ZEND_THIS))->texture storageMode]); }
PHP_METHOD(Metal_Texture, getMipmapLevelCount) { ZEND_PARSE_PARAMETERS_NONE(); RETURN_LONG((zend_long)[metal_texture_from_obj(Z_OBJ_P(ZEND_THIS))->texture mipmapLevelCount]); }
PHP_METHOD(Metal_Texture, getArrayLength) { ZEND_PARSE_PARAMETERS_NONE(); RETURN_LONG((zend_long)[metal_texture_from_obj(Z_OBJ_P(ZEND_THIS))->texture arrayLength]); }
PHP_METHOD(Metal_Texture, getSampleCount) { ZEND_PARSE_PARAMETERS_NONE(); RETURN_LONG((zend_long)[metal_texture_from_obj(Z_OBJ_P(ZEND_THIS))->texture sampleCount]); }
PHP_METHOD(Metal_Texture, isFramebufferOnly) { ZEND_PARSE_PARAMETERS_NONE(); RETURN_BOOL([metal_texture_from_obj(Z_OBJ_P(ZEND_THIS))->texture isFramebufferOnly]); }
PHP_METHOD(Metal_Texture, makeAliasable) { ZEND_PARSE_PARAMETERS_NONE(); [metal_texture_from_obj(Z_OBJ_P(ZEND_THIS))->texture makeAliasable]; }
PHP_METHOD(Metal_Texture, isAliasable) { ZEND_PARSE_PARAMETERS_NONE(); RETURN_BOOL([metal_texture_from_obj(Z_OBJ_P(ZEND_THIS))->texture isAliasable]); }

/* }}} */

/* {{{ Enhanced MetalFunction methods */

PHP_METHOD(Metal_MetalFunction, createArgumentEncoder)
{
    zend_long index;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(index) ZEND_PARSE_PARAMETERS_END();
    metal_function_t *intern = metal_function_from_obj(Z_OBJ_P(ZEND_THIS));
    id<MTLArgumentEncoder> enc = [intern->function newArgumentEncoderWithBufferIndex:(NSUInteger)index];
    if (!enc) { zend_throw_exception(metal_ce_exception, "Failed to create argument encoder", 0); RETURN_THROWS(); }
    METAL_WRAP_RETURN(argument_encoder, metal_argument_encoder_t, encoder, metal_ce_argument_encoder, enc);
}

/* }}} */

/* ====================================================================
 *  Metal\Library methods
 * ==================================================================== */

/* {{{ proto Metal\MetalFunction Metal\Library::getFunction(string $name) */
PHP_METHOD(Metal_Library, getFunction)
{
    zend_string *name;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STR(name)
    ZEND_PARSE_PARAMETERS_END();

    metal_library_t *intern = metal_library_from_obj(Z_OBJ_P(ZEND_THIS));
    NSString *nsname = [NSString stringWithUTF8String:ZSTR_VAL(name)];
    id<MTLFunction> fn = [intern->library newFunctionWithName:nsname];

    if (fn == nil) {
        zend_throw_exception_ex(metal_ce_exception, 0,
            "Function '%s' not found in library", ZSTR_VAL(name));
        RETURN_THROWS();
    }

    METAL_WRAP_RETURN(function, metal_function_t, function, metal_ce_metal_function, fn);
}
/* }}} */

/* {{{ proto array Metal\Library::getFunctionNames() */
PHP_METHOD(Metal_Library, getFunctionNames)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_library_t *intern = metal_library_from_obj(Z_OBJ_P(ZEND_THIS));
    NSArray<NSString *> *names = [intern->library functionNames];

    array_init_size(return_value, (uint32_t)[names count]);
    for (NSString *name in names) {
        add_next_index_string(return_value, [name UTF8String]);
    }
}
/* }}} */

/* ====================================================================
 *  Metal\MetalFunction methods
 * ==================================================================== */

/* {{{ proto string Metal\MetalFunction::getName() */
PHP_METHOD(Metal_MetalFunction, getName)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_function_t *intern = metal_function_from_obj(Z_OBJ_P(ZEND_THIS));
    RETURN_STRING([[intern->function name] UTF8String]);
}
/* }}} */

/* {{{ proto int Metal\MetalFunction::getFunctionType() */
PHP_METHOD(Metal_MetalFunction, getFunctionType)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_function_t *intern = metal_function_from_obj(Z_OBJ_P(ZEND_THIS));
    RETURN_LONG((zend_long)[intern->function functionType]);
}
/* }}} */

/* ====================================================================
 *  Metal\ComputePipelineState methods
 * ==================================================================== */

/* {{{ proto int Metal\ComputePipelineState::getMaxTotalThreadsPerThreadgroup() */
PHP_METHOD(Metal_ComputePipelineState, getMaxTotalThreadsPerThreadgroup)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_compute_pipeline_state_t *intern = metal_compute_pipeline_state_from_obj(Z_OBJ_P(ZEND_THIS));
    RETURN_LONG((zend_long)[intern->state maxTotalThreadsPerThreadgroup]);
}
/* }}} */

/* {{{ proto int Metal\ComputePipelineState::getThreadExecutionWidth() */
PHP_METHOD(Metal_ComputePipelineState, getThreadExecutionWidth)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_compute_pipeline_state_t *intern = metal_compute_pipeline_state_from_obj(Z_OBJ_P(ZEND_THIS));
    RETURN_LONG((zend_long)[intern->state threadExecutionWidth]);
}
/* }}} */

/* ====================================================================
 *  Metal\Texture methods
 * ==================================================================== */

/* {{{ proto int Metal\Texture::getWidth() */
PHP_METHOD(Metal_Texture, getWidth)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_texture_t *intern = metal_texture_from_obj(Z_OBJ_P(ZEND_THIS));
    RETURN_LONG((zend_long)[intern->texture width]);
}
/* }}} */

/* {{{ proto int Metal\Texture::getHeight() */
PHP_METHOD(Metal_Texture, getHeight)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_texture_t *intern = metal_texture_from_obj(Z_OBJ_P(ZEND_THIS));
    RETURN_LONG((zend_long)[intern->texture height]);
}
/* }}} */

/* {{{ proto int Metal\Texture::getDepth() */
PHP_METHOD(Metal_Texture, getDepth)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_texture_t *intern = metal_texture_from_obj(Z_OBJ_P(ZEND_THIS));
    RETURN_LONG((zend_long)[intern->texture depth]);
}
/* }}} */

/* {{{ proto int Metal\Texture::getPixelFormat() */
PHP_METHOD(Metal_Texture, getPixelFormat)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_texture_t *intern = metal_texture_from_obj(Z_OBJ_P(ZEND_THIS));
    RETURN_LONG((zend_long)[intern->texture pixelFormat]);
}
/* }}} */

/* {{{ proto int Metal\Texture::getTextureType() */
PHP_METHOD(Metal_Texture, getTextureType)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_texture_t *intern = metal_texture_from_obj(Z_OBJ_P(ZEND_THIS));
    RETURN_LONG((zend_long)[intern->texture textureType]);
}
/* }}} */

/* {{{ proto void Metal\Texture::replaceRegion(array $region, int $mipmapLevel, string $data, int $bytesPerRow) */
PHP_METHOD(Metal_Texture, replaceRegion)
{
    zval *zregion;
    zend_long mipmap_level;
    zend_string *data;
    zend_long bytes_per_row;

    ZEND_PARSE_PARAMETERS_START(4, 4)
        Z_PARAM_ARRAY(zregion)
        Z_PARAM_LONG(mipmap_level)
        Z_PARAM_STR(data)
        Z_PARAM_LONG(bytes_per_row)
    ZEND_PARSE_PARAMETERS_END();

    metal_texture_t *intern = metal_texture_from_obj(Z_OBJ_P(ZEND_THIS));

    /* region = [x, y, width, height] */
    HashTable *ht = Z_ARRVAL_P(zregion);
    zval *val;
    NSUInteger rx = 0, ry = 0, rw = 1, rh = 1;
    if ((val = zend_hash_index_find(ht, 0)) != NULL) rx = (NSUInteger)zval_get_long(val);
    if ((val = zend_hash_index_find(ht, 1)) != NULL) ry = (NSUInteger)zval_get_long(val);
    if ((val = zend_hash_index_find(ht, 2)) != NULL) rw = (NSUInteger)zval_get_long(val);
    if ((val = zend_hash_index_find(ht, 3)) != NULL) rh = (NSUInteger)zval_get_long(val);

    MTLRegion region = MTLRegionMake2D(rx, ry, rw, rh);

    [intern->texture replaceRegion:region
                       mipmapLevel:(NSUInteger)mipmap_level
                         withBytes:ZSTR_VAL(data)
                       bytesPerRow:(NSUInteger)bytes_per_row];
}
/* }}} */

/* {{{ proto string Metal\Texture::getBytes(array $region, int $mipmapLevel, int $bytesPerRow) */
PHP_METHOD(Metal_Texture, getBytes)
{
    zval *zregion;
    zend_long mipmap_level;
    zend_long bytes_per_row;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_ARRAY(zregion)
        Z_PARAM_LONG(mipmap_level)
        Z_PARAM_LONG(bytes_per_row)
    ZEND_PARSE_PARAMETERS_END();

    metal_texture_t *intern = metal_texture_from_obj(Z_OBJ_P(ZEND_THIS));

    HashTable *ht = Z_ARRVAL_P(zregion);
    zval *val;
    NSUInteger rx = 0, ry = 0, rw = 1, rh = 1;
    if ((val = zend_hash_index_find(ht, 0)) != NULL) rx = (NSUInteger)zval_get_long(val);
    if ((val = zend_hash_index_find(ht, 1)) != NULL) ry = (NSUInteger)zval_get_long(val);
    if ((val = zend_hash_index_find(ht, 2)) != NULL) rw = (NSUInteger)zval_get_long(val);
    if ((val = zend_hash_index_find(ht, 3)) != NULL) rh = (NSUInteger)zval_get_long(val);

    MTLRegion region = MTLRegionMake2D(rx, ry, rw, rh);
    size_t total_bytes = (size_t)bytes_per_row * rh;

    zend_string *result = zend_string_alloc(total_bytes, 0);
    [intern->texture getBytes:ZSTR_VAL(result)
                  bytesPerRow:(NSUInteger)bytes_per_row
                   fromRegion:region
                  mipmapLevel:(NSUInteger)mipmap_level];
    ZSTR_LEN(result) = total_bytes;
    ZSTR_VAL(result)[total_bytes] = '\0';
    RETURN_NEW_STR(result);
}
/* }}} */

/* ====================================================================
 *  Metal\TextureDescriptor methods
 * ==================================================================== */

/* {{{ proto void Metal\TextureDescriptor::__construct() */
PHP_METHOD(Metal_TextureDescriptor, __construct)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_texture_descriptor_t *intern = metal_texture_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor = [[MTLTextureDescriptor alloc] init];
}
/* }}} */

/* {{{ proto static Metal\TextureDescriptor Metal\TextureDescriptor::texture2DDescriptor(int $pixelFormat, int $width, int $height, bool $mipmapped = false) */
PHP_METHOD(Metal_TextureDescriptor, texture2DDescriptor)
{
    zend_long pixel_format, width, height;
    bool mipmapped = false;

    ZEND_PARSE_PARAMETERS_START(3, 4)
        Z_PARAM_LONG(pixel_format)
        Z_PARAM_LONG(width)
        Z_PARAM_LONG(height)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(mipmapped)
    ZEND_PARSE_PARAMETERS_END();

    MTLTextureDescriptor *desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:(MTLPixelFormat)pixel_format
                                                                                    width:(NSUInteger)width
                                                                                   height:(NSUInteger)height
                                                                                mipmapped:mipmapped];

    object_init_ex(return_value, metal_ce_texture_descriptor);
    metal_texture_descriptor_t *intern = metal_texture_descriptor_from_obj(Z_OBJ_P(return_value));
    intern->descriptor = desc;
}
/* }}} */

/* {{{ proto void Metal\TextureDescriptor::setPixelFormat(int $format) */
PHP_METHOD(Metal_TextureDescriptor, setPixelFormat)
{
    zend_long format;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(format) ZEND_PARSE_PARAMETERS_END();
    metal_texture_descriptor_t *intern = metal_texture_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.pixelFormat = (MTLPixelFormat)format;
}
/* }}} */

/* {{{ proto void Metal\TextureDescriptor::setWidth(int $width) */
PHP_METHOD(Metal_TextureDescriptor, setWidth)
{
    zend_long width;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(width) ZEND_PARSE_PARAMETERS_END();
    metal_texture_descriptor_t *intern = metal_texture_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.width = (NSUInteger)width;
}
/* }}} */

/* {{{ proto void Metal\TextureDescriptor::setHeight(int $height) */
PHP_METHOD(Metal_TextureDescriptor, setHeight)
{
    zend_long height;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(height) ZEND_PARSE_PARAMETERS_END();
    metal_texture_descriptor_t *intern = metal_texture_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.height = (NSUInteger)height;
}
/* }}} */

/* {{{ proto void Metal\TextureDescriptor::setTextureType(int $type) */
PHP_METHOD(Metal_TextureDescriptor, setTextureType)
{
    zend_long type;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(type) ZEND_PARSE_PARAMETERS_END();
    metal_texture_descriptor_t *intern = metal_texture_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.textureType = (MTLTextureType)type;
}
/* }}} */

/* {{{ proto void Metal\TextureDescriptor::setUsage(int $usage) */
PHP_METHOD(Metal_TextureDescriptor, setUsage)
{
    zend_long usage;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(usage) ZEND_PARSE_PARAMETERS_END();
    metal_texture_descriptor_t *intern = metal_texture_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.usage = (MTLTextureUsage)usage;
}
/* }}} */

/* {{{ proto void Metal\TextureDescriptor::setStorageMode(int $mode) */
PHP_METHOD(Metal_TextureDescriptor, setStorageMode)
{
    zend_long mode;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(mode) ZEND_PARSE_PARAMETERS_END();
    metal_texture_descriptor_t *intern = metal_texture_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.storageMode = (MTLStorageMode)mode;
}
/* }}} */

/* ====================================================================
 *  Metal\RenderPipelineDescriptor methods
 * ==================================================================== */

/* {{{ proto void Metal\RenderPipelineDescriptor::__construct() */
PHP_METHOD(Metal_RenderPipelineDescriptor, __construct)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_render_pipeline_descriptor_t *intern = metal_render_pipeline_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor = [[MTLRenderPipelineDescriptor alloc] init];
}
/* }}} */

/* {{{ proto void Metal\RenderPipelineDescriptor::setVertexFunction(Metal\MetalFunction $function) */
PHP_METHOD(Metal_RenderPipelineDescriptor, setVertexFunction)
{
    zval *zfunction;
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zfunction, metal_ce_metal_function)
    ZEND_PARSE_PARAMETERS_END();

    metal_render_pipeline_descriptor_t *intern = metal_render_pipeline_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_function_t *fn = metal_function_from_obj(Z_OBJ_P(zfunction));
    intern->descriptor.vertexFunction = fn->function;
}
/* }}} */

/* {{{ proto void Metal\RenderPipelineDescriptor::setFragmentFunction(Metal\MetalFunction $function) */
PHP_METHOD(Metal_RenderPipelineDescriptor, setFragmentFunction)
{
    zval *zfunction;
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zfunction, metal_ce_metal_function)
    ZEND_PARSE_PARAMETERS_END();

    metal_render_pipeline_descriptor_t *intern = metal_render_pipeline_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_function_t *fn = metal_function_from_obj(Z_OBJ_P(zfunction));
    intern->descriptor.fragmentFunction = fn->function;
}
/* }}} */

/* {{{ proto Metal\ColorAttachmentDescriptor Metal\RenderPipelineDescriptor::getColorAttachment(int $index) */
PHP_METHOD(Metal_RenderPipelineDescriptor, getColorAttachment)
{
    zend_long index;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(index) ZEND_PARSE_PARAMETERS_END();

    metal_render_pipeline_descriptor_t *intern = metal_render_pipeline_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));

    if (index < 0 || index >= 8) {
        zend_throw_exception(metal_ce_exception, "Color attachment index must be 0-7", 0);
        RETURN_THROWS();
    }

    MTLRenderPipelineColorAttachmentDescriptor *ca = intern->descriptor.colorAttachments[index];

    object_init_ex(return_value, metal_ce_color_attachment_descriptor);
    metal_color_attachment_descriptor_t *out = metal_color_attachment_descriptor_from_obj(Z_OBJ_P(return_value));
    out->descriptor = ca;
}
/* }}} */

/* {{{ proto void Metal\RenderPipelineDescriptor::setDepthAttachmentPixelFormat(int $format) */
PHP_METHOD(Metal_RenderPipelineDescriptor, setDepthAttachmentPixelFormat)
{
    zend_long format;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(format) ZEND_PARSE_PARAMETERS_END();

    metal_render_pipeline_descriptor_t *intern = metal_render_pipeline_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.depthAttachmentPixelFormat = (MTLPixelFormat)format;
}
/* }}} */

/* {{{ proto void Metal\RenderPipelineDescriptor::setVertexDescriptor(Metal\VertexDescriptor $descriptor) */
PHP_METHOD(Metal_RenderPipelineDescriptor, setVertexDescriptor)
{
    zval *zdescriptor;
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zdescriptor, metal_ce_vertex_descriptor)
    ZEND_PARSE_PARAMETERS_END();

    metal_render_pipeline_descriptor_t *intern = metal_render_pipeline_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_vertex_descriptor_t *vd = metal_vertex_descriptor_from_obj(Z_OBJ_P(zdescriptor));
    intern->descriptor.vertexDescriptor = vd->descriptor;
}
/* }}} */

/* ====================================================================
 *  Metal\ColorAttachmentDescriptor methods
 * ==================================================================== */

/* {{{ proto void Metal\ColorAttachmentDescriptor::setPixelFormat(int $format) */
PHP_METHOD(Metal_ColorAttachmentDescriptor, setPixelFormat)
{
    zend_long format;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(format) ZEND_PARSE_PARAMETERS_END();

    metal_color_attachment_descriptor_t *intern = metal_color_attachment_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.pixelFormat = (MTLPixelFormat)format;
}
/* }}} */

/* {{{ proto void Metal\ColorAttachmentDescriptor::setBlendingEnabled(bool $enabled) */
PHP_METHOD(Metal_ColorAttachmentDescriptor, setBlendingEnabled)
{
    bool enabled;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_BOOL(enabled) ZEND_PARSE_PARAMETERS_END();

    metal_color_attachment_descriptor_t *intern = metal_color_attachment_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.blendingEnabled = enabled;
}
/* }}} */

/* {{{ proto void Metal\ColorAttachmentDescriptor::setSourceRGBBlendFactor(int $factor) */
PHP_METHOD(Metal_ColorAttachmentDescriptor, setSourceRGBBlendFactor)
{
    zend_long factor;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(factor) ZEND_PARSE_PARAMETERS_END();
    metal_color_attachment_descriptor_t *intern = metal_color_attachment_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.sourceRGBBlendFactor = (MTLBlendFactor)factor;
}
/* }}} */

/* {{{ proto void Metal\ColorAttachmentDescriptor::setDestinationRGBBlendFactor(int $factor) */
PHP_METHOD(Metal_ColorAttachmentDescriptor, setDestinationRGBBlendFactor)
{
    zend_long factor;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(factor) ZEND_PARSE_PARAMETERS_END();
    metal_color_attachment_descriptor_t *intern = metal_color_attachment_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.destinationRGBBlendFactor = (MTLBlendFactor)factor;
}
/* }}} */

/* {{{ proto void Metal\ColorAttachmentDescriptor::setSourceAlphaBlendFactor(int $factor) */
PHP_METHOD(Metal_ColorAttachmentDescriptor, setSourceAlphaBlendFactor)
{
    zend_long factor;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(factor) ZEND_PARSE_PARAMETERS_END();
    metal_color_attachment_descriptor_t *intern = metal_color_attachment_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.sourceAlphaBlendFactor = (MTLBlendFactor)factor;
}
/* }}} */

/* {{{ proto void Metal\ColorAttachmentDescriptor::setDestinationAlphaBlendFactor(int $factor) */
PHP_METHOD(Metal_ColorAttachmentDescriptor, setDestinationAlphaBlendFactor)
{
    zend_long factor;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(factor) ZEND_PARSE_PARAMETERS_END();
    metal_color_attachment_descriptor_t *intern = metal_color_attachment_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.destinationAlphaBlendFactor = (MTLBlendFactor)factor;
}
/* }}} */

/* ====================================================================
 *  Metal\RenderPassDescriptor methods
 * ==================================================================== */

/* {{{ proto void Metal\RenderPassDescriptor::__construct() */
PHP_METHOD(Metal_RenderPassDescriptor, __construct)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_render_pass_descriptor_t *intern = metal_render_pass_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor = [[MTLRenderPassDescriptor alloc] init];
}
/* }}} */

/* {{{ proto void Metal\RenderPassDescriptor::setColorAttachmentTexture(int $index, Metal\Texture $texture) */
PHP_METHOD(Metal_RenderPassDescriptor, setColorAttachmentTexture)
{
    zend_long index;
    zval *ztexture;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_LONG(index)
        Z_PARAM_OBJECT_OF_CLASS(ztexture, metal_ce_texture)
    ZEND_PARSE_PARAMETERS_END();

    metal_render_pass_descriptor_t *intern = metal_render_pass_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_texture_t *tex = metal_texture_from_obj(Z_OBJ_P(ztexture));

    intern->descriptor.colorAttachments[index].texture = tex->texture;
}
/* }}} */

/* {{{ proto void Metal\RenderPassDescriptor::setColorAttachmentLoadAction(int $index, int $action) */
PHP_METHOD(Metal_RenderPassDescriptor, setColorAttachmentLoadAction)
{
    zend_long index, action;
    ZEND_PARSE_PARAMETERS_START(2, 2) Z_PARAM_LONG(index) Z_PARAM_LONG(action) ZEND_PARSE_PARAMETERS_END();

    metal_render_pass_descriptor_t *intern = metal_render_pass_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.colorAttachments[index].loadAction = (MTLLoadAction)action;
}
/* }}} */

/* {{{ proto void Metal\RenderPassDescriptor::setColorAttachmentStoreAction(int $index, int $action) */
PHP_METHOD(Metal_RenderPassDescriptor, setColorAttachmentStoreAction)
{
    zend_long index, action;
    ZEND_PARSE_PARAMETERS_START(2, 2) Z_PARAM_LONG(index) Z_PARAM_LONG(action) ZEND_PARSE_PARAMETERS_END();

    metal_render_pass_descriptor_t *intern = metal_render_pass_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.colorAttachments[index].storeAction = (MTLStoreAction)action;
}
/* }}} */

/* {{{ proto void Metal\RenderPassDescriptor::setColorAttachmentClearColor(int $index, float $r, float $g, float $b, float $a) */
PHP_METHOD(Metal_RenderPassDescriptor, setColorAttachmentClearColor)
{
    zend_long index;
    double r, g, b, a;

    ZEND_PARSE_PARAMETERS_START(5, 5)
        Z_PARAM_LONG(index)
        Z_PARAM_DOUBLE(r)
        Z_PARAM_DOUBLE(g)
        Z_PARAM_DOUBLE(b)
        Z_PARAM_DOUBLE(a)
    ZEND_PARSE_PARAMETERS_END();

    metal_render_pass_descriptor_t *intern = metal_render_pass_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.colorAttachments[index].clearColor = MTLClearColorMake(r, g, b, a);
}
/* }}} */

/* {{{ proto void Metal\RenderPassDescriptor::setDepthAttachmentTexture(Metal\Texture $texture) */
PHP_METHOD(Metal_RenderPassDescriptor, setDepthAttachmentTexture)
{
    zval *ztexture;
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(ztexture, metal_ce_texture)
    ZEND_PARSE_PARAMETERS_END();

    metal_render_pass_descriptor_t *intern = metal_render_pass_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    metal_texture_t *tex = metal_texture_from_obj(Z_OBJ_P(ztexture));
    intern->descriptor.depthAttachment.texture = tex->texture;
}
/* }}} */

/* {{{ proto void Metal\RenderPassDescriptor::setDepthAttachmentLoadAction(int $action) */
PHP_METHOD(Metal_RenderPassDescriptor, setDepthAttachmentLoadAction)
{
    zend_long action;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(action) ZEND_PARSE_PARAMETERS_END();

    metal_render_pass_descriptor_t *intern = metal_render_pass_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.depthAttachment.loadAction = (MTLLoadAction)action;
}
/* }}} */

/* {{{ proto void Metal\RenderPassDescriptor::setDepthAttachmentStoreAction(int $action) */
PHP_METHOD(Metal_RenderPassDescriptor, setDepthAttachmentStoreAction)
{
    zend_long action;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(action) ZEND_PARSE_PARAMETERS_END();

    metal_render_pass_descriptor_t *intern = metal_render_pass_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.depthAttachment.storeAction = (MTLStoreAction)action;
}
/* }}} */

/* {{{ proto void Metal\RenderPassDescriptor::setDepthAttachmentClearDepth(float $depth) */
PHP_METHOD(Metal_RenderPassDescriptor, setDepthAttachmentClearDepth)
{
    double depth;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_DOUBLE(depth) ZEND_PARSE_PARAMETERS_END();

    metal_render_pass_descriptor_t *intern = metal_render_pass_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.depthAttachment.clearDepth = depth;
}
/* }}} */

/* ====================================================================
 *  Metal\DepthStencilDescriptor methods
 * ==================================================================== */

/* {{{ proto void Metal\DepthStencilDescriptor::__construct() */
PHP_METHOD(Metal_DepthStencilDescriptor, __construct)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_depth_stencil_descriptor_t *intern = metal_depth_stencil_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor = [[MTLDepthStencilDescriptor alloc] init];
}
/* }}} */

/* {{{ proto void Metal\DepthStencilDescriptor::setDepthCompareFunction(int $function) */
PHP_METHOD(Metal_DepthStencilDescriptor, setDepthCompareFunction)
{
    zend_long function;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(function) ZEND_PARSE_PARAMETERS_END();
    metal_depth_stencil_descriptor_t *intern = metal_depth_stencil_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.depthCompareFunction = (MTLCompareFunction)function;
}
/* }}} */

/* {{{ proto void Metal\DepthStencilDescriptor::setDepthWriteEnabled(bool $enabled) */
PHP_METHOD(Metal_DepthStencilDescriptor, setDepthWriteEnabled)
{
    bool enabled;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_BOOL(enabled) ZEND_PARSE_PARAMETERS_END();
    metal_depth_stencil_descriptor_t *intern = metal_depth_stencil_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.depthWriteEnabled = enabled;
}
/* }}} */

/* ====================================================================
 *  Metal\VertexDescriptor methods
 * ==================================================================== */

/* {{{ proto void Metal\VertexDescriptor::__construct() */
PHP_METHOD(Metal_VertexDescriptor, __construct)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_vertex_descriptor_t *intern = metal_vertex_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor = [[MTLVertexDescriptor alloc] init];
}
/* }}} */

/* {{{ proto void Metal\VertexDescriptor::setAttribute(int $index, int $format, int $offset, int $bufferIndex) */
PHP_METHOD(Metal_VertexDescriptor, setAttribute)
{
    zend_long index, format, offset, buffer_index;

    ZEND_PARSE_PARAMETERS_START(4, 4)
        Z_PARAM_LONG(index)
        Z_PARAM_LONG(format)
        Z_PARAM_LONG(offset)
        Z_PARAM_LONG(buffer_index)
    ZEND_PARSE_PARAMETERS_END();

    metal_vertex_descriptor_t *intern = metal_vertex_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.attributes[index].format = (MTLVertexFormat)format;
    intern->descriptor.attributes[index].offset = (NSUInteger)offset;
    intern->descriptor.attributes[index].bufferIndex = (NSUInteger)buffer_index;
}
/* }}} */

/* {{{ proto void Metal\VertexDescriptor::setLayout(int $index, int $stride, int $stepFunction = 1, int $stepRate = 1) */
PHP_METHOD(Metal_VertexDescriptor, setLayout)
{
    zend_long index, stride;
    zend_long step_function = MTLVertexStepFunctionPerVertex;
    zend_long step_rate = 1;

    ZEND_PARSE_PARAMETERS_START(2, 4)
        Z_PARAM_LONG(index)
        Z_PARAM_LONG(stride)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(step_function)
        Z_PARAM_LONG(step_rate)
    ZEND_PARSE_PARAMETERS_END();

    metal_vertex_descriptor_t *intern = metal_vertex_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.layouts[index].stride = (NSUInteger)stride;
    intern->descriptor.layouts[index].stepFunction = (MTLVertexStepFunction)step_function;
    intern->descriptor.layouts[index].stepRate = (NSUInteger)step_rate;
}
/* }}} */

/* ====================================================================
 *  Metal\SamplerDescriptor methods
 * ==================================================================== */

/* {{{ proto void Metal\SamplerDescriptor::__construct() */
PHP_METHOD(Metal_SamplerDescriptor, __construct)
{
    ZEND_PARSE_PARAMETERS_NONE();
    metal_sampler_descriptor_t *intern = metal_sampler_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor = [[MTLSamplerDescriptor alloc] init];
}
/* }}} */

/* {{{ proto void Metal\SamplerDescriptor::setMinFilter(int $filter) */
PHP_METHOD(Metal_SamplerDescriptor, setMinFilter)
{
    zend_long filter;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(filter) ZEND_PARSE_PARAMETERS_END();
    metal_sampler_descriptor_t *intern = metal_sampler_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.minFilter = (MTLSamplerMinMagFilter)filter;
}
/* }}} */

/* {{{ proto void Metal\SamplerDescriptor::setMagFilter(int $filter) */
PHP_METHOD(Metal_SamplerDescriptor, setMagFilter)
{
    zend_long filter;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(filter) ZEND_PARSE_PARAMETERS_END();
    metal_sampler_descriptor_t *intern = metal_sampler_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.magFilter = (MTLSamplerMinMagFilter)filter;
}
/* }}} */

/* {{{ proto void Metal\SamplerDescriptor::setSAddressMode(int $mode) */
PHP_METHOD(Metal_SamplerDescriptor, setSAddressMode)
{
    zend_long mode;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(mode) ZEND_PARSE_PARAMETERS_END();
    metal_sampler_descriptor_t *intern = metal_sampler_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.sAddressMode = (MTLSamplerAddressMode)mode;
}
/* }}} */

/* {{{ proto void Metal\SamplerDescriptor::setTAddressMode(int $mode) */
PHP_METHOD(Metal_SamplerDescriptor, setTAddressMode)
{
    zend_long mode;
    ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(mode) ZEND_PARSE_PARAMETERS_END();
    metal_sampler_descriptor_t *intern = metal_sampler_descriptor_from_obj(Z_OBJ_P(ZEND_THIS));
    intern->descriptor.tAddressMode = (MTLSamplerAddressMode)mode;
}
/* }}} */

/* ====================================================================
 *  Arginfo — method signatures for reflection and type checking
 * ==================================================================== */

/* --- Free functions --- */
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_Metal_createSystemDefaultDevice, 0, 0, Metal\\Device, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_allDevices, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

/* --- Device --- */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_Device_getName, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_Device_isLowPower, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_Device_isRemovable, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_Device_getRegistryID, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_Device_getMaxThreadsPerThreadgroup, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_Device_getMaxThreadgroupMemoryLength, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_Device_getMaxBufferLength, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_Device_supportsFamily, 0, 1, _IS_BOOL, 0)
    ZEND_ARG_TYPE_INFO(0, family, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_Metal_Device_createBuffer, 0, 1, Metal\\Buffer, 0)
    ZEND_ARG_TYPE_INFO(0, length, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_LONG, 0, "0")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_Metal_Device_createBufferFromData, 0, 1, Metal\\Buffer, 0)
    ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, type, IS_LONG, 0, "Metal\\FLOAT")
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_LONG, 0, "0")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_Metal_Device_createCommandQueue, 0, 0, Metal\\CommandQueue, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_Metal_Device_createLibraryWithSource, 0, 1, Metal\\Library, 0)
    ZEND_ARG_TYPE_INFO(0, source, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_Metal_Device_createLibraryWithFile, 0, 1, Metal\\Library, 0)
    ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_Metal_Device_createDefaultLibrary, 0, 0, Metal\\Library, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_Metal_Device_createComputePipelineState, 0, 1, Metal\\ComputePipelineState, 0)
    ZEND_ARG_OBJ_INFO(0, function, Metal\\MetalFunction, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_Metal_Device_createTexture, 0, 1, Metal\\Texture, 0)
    ZEND_ARG_OBJ_INFO(0, descriptor, Metal\\TextureDescriptor, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_Metal_Device_createDepthStencilState, 0, 1, Metal\\DepthStencilState, 0)
    ZEND_ARG_OBJ_INFO(0, descriptor, Metal\\DepthStencilDescriptor, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_Metal_Device_createRenderPipelineState, 0, 1, Metal\\RenderPipelineState, 0)
    ZEND_ARG_OBJ_INFO(0, descriptor, Metal\\RenderPipelineDescriptor, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_Metal_Device_createSamplerState, 0, 1, Metal\\SamplerState, 0)
    ZEND_ARG_OBJ_INFO(0, descriptor, Metal\\SamplerDescriptor, 0)
ZEND_END_ARG_INFO()

/* --- Buffer --- */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_Buffer_getLength, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_Buffer_getContents, 0, 0, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, type, IS_LONG, 0, "Metal\\FLOAT")
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, count, IS_LONG, 0, "-1")
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, offset, IS_LONG, 0, "0")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_Buffer_writeContents, 0, 1, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, type, IS_LONG, 0, "Metal\\FLOAT")
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, offset, IS_LONG, 0, "0")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_Buffer_getRawContents, 0, 0, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, length, IS_LONG, 0, "-1")
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, offset, IS_LONG, 0, "0")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_Buffer_writeRawContents, 0, 1, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, offset, IS_LONG, 0, "0")
ZEND_END_ARG_INFO()

/* --- CommandQueue --- */
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_Metal_CommandQueue_createCommandBuffer, 0, 0, Metal\\CommandBuffer, 0)
ZEND_END_ARG_INFO()

/* --- CommandBuffer --- */
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_Metal_CommandBuffer_createComputeCommandEncoder, 0, 0, Metal\\ComputeCommandEncoder, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_Metal_CommandBuffer_createRenderCommandEncoder, 0, 1, Metal\\RenderCommandEncoder, 0)
    ZEND_ARG_OBJ_INFO(0, descriptor, Metal\\RenderPassDescriptor, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_Metal_CommandBuffer_createBlitCommandEncoder, 0, 0, Metal\\BlitCommandEncoder, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_void, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_CommandBuffer_getStatus, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_CommandBuffer_presentDrawable, 0, 1, IS_VOID, 0)
    ZEND_ARG_OBJ_INFO(0, drawable, Metal\\Drawable, 0)
ZEND_END_ARG_INFO()

/* --- ComputeCommandEncoder --- */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_ComputeCommandEncoder_setComputePipelineState, 0, 1, IS_VOID, 0)
    ZEND_ARG_OBJ_INFO(0, state, Metal\\ComputePipelineState, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_ComputeCommandEncoder_setBuffer, 0, 3, IS_VOID, 0)
    ZEND_ARG_OBJ_INFO(0, buffer, Metal\\Buffer, 0)
    ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_ComputeCommandEncoder_setBytes, 0, 2, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_Encoder_setTexture, 0, 2, IS_VOID, 0)
    ZEND_ARG_OBJ_INFO(0, texture, Metal\\Texture, 0)
    ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_Encoder_setSamplerState, 0, 2, IS_VOID, 0)
    ZEND_ARG_OBJ_INFO(0, sampler, Metal\\SamplerState, 0)
    ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_ComputeCommandEncoder_setThreadgroupMemoryLength, 0, 2, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, length, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_ComputeCommandEncoder_dispatch, 0, 2, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, gridSize, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO(0, threadgroupSize, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

/* --- Library --- */
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_Metal_Library_getFunction, 0, 1, Metal\\MetalFunction, 0)
    ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_Library_getFunctionNames, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

/* --- MetalFunction --- */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_MetalFunction_getName, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_MetalFunction_getFunctionType, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* --- ComputePipelineState --- */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_ComputePipelineState_getMaxTotalThreadsPerThreadgroup, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_ComputePipelineState_getThreadExecutionWidth, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* --- Texture --- */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_Texture_getDimension, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_Texture_replaceRegion, 0, 4, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, region, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO(0, mipmapLevel, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, bytesPerRow, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_Texture_getBytes, 0, 3, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, region, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO(0, mipmapLevel, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, bytesPerRow, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* --- TextureDescriptor --- */
ZEND_BEGIN_ARG_INFO_EX(arginfo_Metal_TextureDescriptor___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_Metal_TextureDescriptor_texture2DDescriptor, 0, 3, Metal\\TextureDescriptor, 0)
    ZEND_ARG_TYPE_INFO(0, pixelFormat, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, width, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, height, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, mipmapped, _IS_BOOL, 0, "false")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_setLong, 0, 1, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_setBool, 0, 1, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, value, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* --- RenderPipelineDescriptor --- */
ZEND_BEGIN_ARG_INFO_EX(arginfo_Metal_RenderPipelineDescriptor___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_RenderPipelineDescriptor_setFunction, 0, 1, IS_VOID, 0)
    ZEND_ARG_OBJ_INFO(0, function, Metal\\MetalFunction, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_Metal_RenderPipelineDescriptor_getColorAttachment, 0, 1, Metal\\ColorAttachmentDescriptor, 0)
    ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_RenderPipelineDescriptor_setVertexDescriptor, 0, 1, IS_VOID, 0)
    ZEND_ARG_OBJ_INFO(0, descriptor, Metal\\VertexDescriptor, 0)
ZEND_END_ARG_INFO()

/* --- RenderPassDescriptor --- */
ZEND_BEGIN_ARG_INFO_EX(arginfo_Metal_RenderPassDescriptor___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_RenderPassDescriptor_setColorAttachmentTexture, 0, 2, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
    ZEND_ARG_OBJ_INFO(0, texture, Metal\\Texture, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_RenderPassDescriptor_setColorAttachmentAction, 0, 2, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, action, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_RenderPassDescriptor_setColorAttachmentClearColor, 0, 5, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, r, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, g, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, b, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_RenderPassDescriptor_setDepthAttachmentTexture, 0, 1, IS_VOID, 0)
    ZEND_ARG_OBJ_INFO(0, texture, Metal\\Texture, 0)
ZEND_END_ARG_INFO()

/* --- RenderCommandEncoder --- */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_RenderCommandEncoder_setRenderPipelineState, 0, 1, IS_VOID, 0)
    ZEND_ARG_OBJ_INFO(0, state, Metal\\RenderPipelineState, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_RenderCommandEncoder_setVertexBuffer, 0, 3, IS_VOID, 0)
    ZEND_ARG_OBJ_INFO(0, buffer, Metal\\Buffer, 0)
    ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_RenderCommandEncoder_setDepthStencilState, 0, 1, IS_VOID, 0)
    ZEND_ARG_OBJ_INFO(0, state, Metal\\DepthStencilState, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_RenderCommandEncoder_setViewport, 0, 6, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, width, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, height, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, znear, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, zfar, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_RenderCommandEncoder_setScissorRect, 0, 4, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, x, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, y, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, width, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, height, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_RenderCommandEncoder_drawPrimitives, 0, 3, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, vertexStart, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, vertexCount, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_RenderCommandEncoder_drawIndexedPrimitives, 0, 5, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, indexCount, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, indexType, IS_LONG, 0)
    ZEND_ARG_OBJ_INFO(0, indexBuffer, Metal\\Buffer, 0)
    ZEND_ARG_TYPE_INFO(0, indexBufferOffset, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* --- BlitCommandEncoder --- */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_BlitCommandEncoder_copyFromBuffer, 0, 5, IS_VOID, 0)
    ZEND_ARG_OBJ_INFO(0, source, Metal\\Buffer, 0)
    ZEND_ARG_TYPE_INFO(0, sourceOffset, IS_LONG, 0)
    ZEND_ARG_OBJ_INFO(0, destination, Metal\\Buffer, 0)
    ZEND_ARG_TYPE_INFO(0, destinationOffset, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, size, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_BlitCommandEncoder_synchronizeResource, 0, 1, IS_VOID, 0)
    ZEND_ARG_OBJ_INFO(0, buffer, Metal\\Buffer, 0)
ZEND_END_ARG_INFO()

/* --- VertexDescriptor --- */
ZEND_BEGIN_ARG_INFO_EX(arginfo_Metal_VertexDescriptor___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_VertexDescriptor_setAttribute, 0, 4, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, format, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, bufferIndex, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Metal_VertexDescriptor_setLayout, 0, 2, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, stride, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, stepFunction, IS_LONG, 0, "1")
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, stepRate, IS_LONG, 0, "1")
ZEND_END_ARG_INFO()

/* --- DepthStencilDescriptor --- */
ZEND_BEGIN_ARG_INFO_EX(arginfo_Metal_DepthStencilDescriptor___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

/* --- SamplerDescriptor --- */
ZEND_BEGIN_ARG_INFO_EX(arginfo_Metal_SamplerDescriptor___construct, 0, 0, 0)
ZEND_END_ARG_INFO()


/* ====================================================================
 *  Arginfo for enhanced/advanced methods
 * ==================================================================== */

/* Shared simple arginfos */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_adv_void, 0, 0, IS_VOID, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_adv_long, 0, 0, IS_LONG, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_adv_bool, 0, 0, _IS_BOOL, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_adv_array, 0, 0, IS_ARRAY, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_adv_void_1long, 0, 1, IS_VOID, 0) ZEND_ARG_TYPE_INFO(0, v, IS_LONG, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_adv_void_2long, 0, 2, IS_VOID, 0) ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0) ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0) ZEND_END_ARG_INFO()

/* Device advanced factory methods */
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_dev_createEvent, 0, 0, Metal\\Event, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_dev_createSharedEvent, 0, 0, Metal\\SharedEvent, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_dev_createFence, 0, 0, Metal\\Fence, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_dev_createHeap, 0, 1, Metal\\Heap, 0) ZEND_ARG_OBJ_INFO(0, d, Metal\\HeapDescriptor, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_dev_createICB, 0, 2, Metal\\IndirectCommandBuffer, 0)
    ZEND_ARG_OBJ_INFO(0, d, Metal\\IndirectCommandBufferDescriptor, 0) ZEND_ARG_TYPE_INFO(0, maxCount, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_LONG, 0, "0")
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_dev_createBA, 0, 1, Metal\\BinaryArchive, 0) ZEND_ARG_OBJ_INFO(0, d, Metal\\BinaryArchiveDescriptor, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_dev_createAS, 0, 1, Metal\\AccelerationStructure, 0) ZEND_ARG_TYPE_INFO(0, size, IS_LONG, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_dev_getASSizes, 0, 1, IS_ARRAY, 0) ZEND_ARG_TYPE_INFO(0, descriptor, IS_OBJECT, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_dev_heapBufSize, 0, 1, IS_ARRAY, 0) ZEND_ARG_TYPE_INFO(0, length, IS_LONG, 0) ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_LONG, 0, "0") ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_dev_heapTexSize, 0, 1, IS_ARRAY, 0) ZEND_ARG_OBJ_INFO(0, d, Metal\\TextureDescriptor, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_dev_createMeshPSO, 0, 1, Metal\\RenderPipelineState, 0) ZEND_ARG_OBJ_INFO(0, d, Metal\\MeshRenderPipelineDescriptor, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_cmdbuf_createASEncoder, 0, 0, Metal\\AccelerationStructureCommandEncoder, 0) ZEND_END_ARG_INFO()

/* Buffer enhanced */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_buf_didModify, 0, 2, IS_VOID, 0) ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 0) ZEND_ARG_TYPE_INFO(0, length, IS_LONG, 0) ZEND_END_ARG_INFO()

/* CommandBuffer event encoding */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_cmdbuf_encodeEvent, 0, 2, IS_VOID, 0) ZEND_ARG_TYPE_INFO(0, event, IS_OBJECT, 0) ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_cmdbuf_createCEwithDesc, 0, 1, Metal\\ComputeCommandEncoder, 0) ZEND_ARG_OBJ_INFO(0, d, Metal\\ComputePassDescriptor, 0) ZEND_END_ARG_INFO()

/* ComputeEncoder enhanced */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_ce_indirectDispatch, 0, 3, IS_VOID, 0) ZEND_ARG_OBJ_INFO(0, buf, Metal\\Buffer, 0) ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 0) ZEND_ARG_TYPE_INFO(0, threadgroupSize, IS_ARRAY, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_ce_useResource, 0, 2, IS_VOID, 0) ZEND_ARG_OBJ_INFO(0, r, Metal\\Buffer, 0) ZEND_ARG_TYPE_INFO(0, usage, IS_LONG, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_ce_fence, 0, 1, IS_VOID, 0) ZEND_ARG_OBJ_INFO(0, fence, Metal\\Fence, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_ce_setAS, 0, 2, IS_VOID, 0) ZEND_ARG_OBJ_INFO(0, as, Metal\\AccelerationStructure, 0) ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0) ZEND_END_ARG_INFO()

/* RenderEncoder enhanced */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_re_drawInstanced, 0, 4, IS_VOID, 0) ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0) ZEND_ARG_TYPE_INFO(0, start, IS_LONG, 0) ZEND_ARG_TYPE_INFO(0, count, IS_LONG, 0) ZEND_ARG_TYPE_INFO(0, instances, IS_LONG, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_re_drawIdxInstanced, 0, 6, IS_VOID, 0) ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0) ZEND_ARG_TYPE_INFO(0, indexCount, IS_LONG, 0) ZEND_ARG_TYPE_INFO(0, indexType, IS_LONG, 0) ZEND_ARG_OBJ_INFO(0, ibo, Metal\\Buffer, 0) ZEND_ARG_TYPE_INFO(0, iboOff, IS_LONG, 0) ZEND_ARG_TYPE_INFO(0, instances, IS_LONG, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_re_drawIndirect, 0, 3, IS_VOID, 0) ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0) ZEND_ARG_OBJ_INFO(0, buf, Metal\\Buffer, 0) ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_re_drawIdxIndirect, 0, 6, IS_VOID, 0) ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0) ZEND_ARG_TYPE_INFO(0, indexType, IS_LONG, 0) ZEND_ARG_OBJ_INFO(0, ibo, Metal\\Buffer, 0) ZEND_ARG_TYPE_INFO(0, iboOff, IS_LONG, 0) ZEND_ARG_OBJ_INFO(0, buf, Metal\\Buffer, 0) ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_re_blendColor, 0, 4, IS_VOID, 0) ZEND_ARG_TYPE_INFO(0, r, IS_DOUBLE, 0) ZEND_ARG_TYPE_INFO(0, g, IS_DOUBLE, 0) ZEND_ARG_TYPE_INFO(0, b, IS_DOUBLE, 0) ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_re_depthBias, 0, 3, IS_VOID, 0) ZEND_ARG_TYPE_INFO(0, bias, IS_DOUBLE, 0) ZEND_ARG_TYPE_INFO(0, slope, IS_DOUBLE, 0) ZEND_ARG_TYPE_INFO(0, clamp, IS_DOUBLE, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_re_fence, 0, 2, IS_VOID, 0) ZEND_ARG_OBJ_INFO(0, fence, Metal\\Fence, 0) ZEND_ARG_TYPE_INFO(0, stages, IS_LONG, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_re_useResource, 0, 3, IS_VOID, 0) ZEND_ARG_OBJ_INFO(0, r, Metal\\Buffer, 0) ZEND_ARG_TYPE_INFO(0, usage, IS_LONG, 0) ZEND_ARG_TYPE_INFO(0, stages, IS_LONG, 0) ZEND_END_ARG_INFO()

/* BlitEncoder enhanced */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_blit_genMips, 0, 1, IS_VOID, 0) ZEND_ARG_OBJ_INFO(0, tex, Metal\\Texture, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_blit_fill, 0, 4, IS_VOID, 0) ZEND_ARG_OBJ_INFO(0, buf, Metal\\Buffer, 0) ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 0) ZEND_ARG_TYPE_INFO(0, length, IS_LONG, 0) ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_blit_copyTex, 0, 8, IS_VOID, 0) ZEND_ARG_OBJ_INFO(0, src, Metal\\Texture, 0) ZEND_ARG_TYPE_INFO(0, srcSlice, IS_LONG, 0) ZEND_ARG_TYPE_INFO(0, srcLevel, IS_LONG, 0) ZEND_ARG_TYPE_INFO(0, srcOrigin, IS_ARRAY, 0) ZEND_ARG_TYPE_INFO(0, srcSize, IS_ARRAY, 0) ZEND_ARG_OBJ_INFO(0, dst, Metal\\Texture, 0) ZEND_ARG_TYPE_INFO(0, dstSlice, IS_LONG, 0) ZEND_ARG_TYPE_INFO(0, dstLevel, IS_LONG, 0) ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_blit_buildAS, 0, 3, IS_VOID, 0) ZEND_ARG_OBJ_INFO(0, as, Metal\\AccelerationStructure, 0) ZEND_ARG_TYPE_INFO(0, desc, IS_OBJECT, 0) ZEND_ARG_OBJ_INFO(0, scratch, Metal\\Buffer, 0) ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, scratchOffset, IS_LONG, 0, "0") ZEND_END_ARG_INFO()

/* Texture enhanced */
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_tex_newView, 0, 1, Metal\\Texture, 0) ZEND_ARG_TYPE_INFO(0, format, IS_LONG, 0) ZEND_END_ARG_INFO()

/* MetalFunction enhanced */
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_fn_createArgEnc, 0, 1, Metal\\ArgumentEncoder, 0) ZEND_ARG_TYPE_INFO(0, bufferIndex, IS_LONG, 0) ZEND_END_ARG_INFO()

/* ====================================================================
 *  Method tables
 * ==================================================================== */

static const zend_function_entry metal_device_methods[] = {
    PHP_ME(Metal_Device, getName,                      arginfo_Metal_Device_getName,                      ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, isLowPower,                   arginfo_Metal_Device_isLowPower,                   ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, isRemovable,                  arginfo_Metal_Device_isRemovable,                  ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, getRegistryID,                arginfo_Metal_Device_getRegistryID,                ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, getMaxThreadsPerThreadgroup,  arginfo_Metal_Device_getMaxThreadsPerThreadgroup,  ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, getMaxThreadgroupMemoryLength,arginfo_Metal_Device_getMaxThreadgroupMemoryLength,ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, getMaxBufferLength,           arginfo_Metal_Device_getMaxBufferLength,           ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, supportsFamily,               arginfo_Metal_Device_supportsFamily,               ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, createBuffer,                 arginfo_Metal_Device_createBuffer,                 ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, createBufferFromData,         arginfo_Metal_Device_createBufferFromData,         ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, createCommandQueue,           arginfo_Metal_Device_createCommandQueue,           ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, createLibraryWithSource,      arginfo_Metal_Device_createLibraryWithSource,      ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, createLibraryWithFile,        arginfo_Metal_Device_createLibraryWithFile,        ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, createDefaultLibrary,         arginfo_Metal_Device_createDefaultLibrary,         ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, createComputePipelineState,   arginfo_Metal_Device_createComputePipelineState,   ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, createTexture,                arginfo_Metal_Device_createTexture,                ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, createDepthStencilState,      arginfo_Metal_Device_createDepthStencilState,      ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, createRenderPipelineState,    arginfo_Metal_Device_createRenderPipelineState,    ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, createSamplerState,           arginfo_Metal_Device_createSamplerState,           ZEND_ACC_PUBLIC)
    /* Advanced factory methods */
    PHP_ME(Metal_Device, createEvent,                        arginfo_dev_createEvent,       ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, createSharedEvent,                  arginfo_dev_createSharedEvent, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, createFence,                        arginfo_dev_createFence,       ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, createHeap,                         arginfo_dev_createHeap,        ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, createIndirectCommandBuffer,        arginfo_dev_createICB,         ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, createBinaryArchive,                arginfo_dev_createBA,          ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, createAccelerationStructureWithSize,arginfo_dev_createAS,          ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, getAccelerationStructureSizes,      arginfo_dev_getASSizes,        ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, heapBufferSizeAndAlign,             arginfo_dev_heapBufSize,       ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, heapTextureSizeAndAlign,            arginfo_dev_heapTexSize,       ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Device, createRenderPipelineStateWithMeshDescriptor, arginfo_dev_createMeshPSO, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry metal_buffer_methods[] = {
    PHP_ME(Metal_Buffer, getLength,        arginfo_Metal_Buffer_getLength,        ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Buffer, getContents,      arginfo_Metal_Buffer_getContents,      ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Buffer, writeContents,    arginfo_Metal_Buffer_writeContents,    ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Buffer, getRawContents,   arginfo_Metal_Buffer_getRawContents,   ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Buffer, writeRawContents, arginfo_Metal_Buffer_writeRawContents, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Buffer, didModifyRange,  arginfo_buf_didModify,                ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Buffer, getGpuAddress,   arginfo_adv_long,                     ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry metal_command_queue_methods[] = {
    PHP_ME(Metal_CommandQueue, createCommandBuffer, arginfo_Metal_CommandQueue_createCommandBuffer, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry metal_command_buffer_methods[] = {
    PHP_ME(Metal_CommandBuffer, createComputeCommandEncoder, arginfo_Metal_CommandBuffer_createComputeCommandEncoder, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_CommandBuffer, createRenderCommandEncoder,  arginfo_Metal_CommandBuffer_createRenderCommandEncoder,  ZEND_ACC_PUBLIC)
    PHP_ME(Metal_CommandBuffer, createBlitCommandEncoder,    arginfo_Metal_CommandBuffer_createBlitCommandEncoder,    ZEND_ACC_PUBLIC)
    PHP_ME(Metal_CommandBuffer, commit,                      arginfo_void,                                            ZEND_ACC_PUBLIC)
    PHP_ME(Metal_CommandBuffer, waitUntilCompleted,          arginfo_void,                                            ZEND_ACC_PUBLIC)
    PHP_ME(Metal_CommandBuffer, waitUntilScheduled,          arginfo_void,                                            ZEND_ACC_PUBLIC)
    PHP_ME(Metal_CommandBuffer, getStatus,                   arginfo_Metal_CommandBuffer_getStatus,                   ZEND_ACC_PUBLIC)
    PHP_ME(Metal_CommandBuffer, presentDrawable,             arginfo_Metal_CommandBuffer_presentDrawable,             ZEND_ACC_PUBLIC)
    PHP_ME(Metal_CommandBuffer, encodeSignalEvent,         arginfo_cmdbuf_encodeEvent,   ZEND_ACC_PUBLIC)
    PHP_ME(Metal_CommandBuffer, encodeWaitForEvent,        arginfo_cmdbuf_encodeEvent,   ZEND_ACC_PUBLIC)
    PHP_ME(Metal_CommandBuffer, createComputeCommandEncoderWithDescriptor,    arginfo_cmdbuf_createCEwithDesc, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_CommandBuffer, createAccelerationStructureCommandEncoder,  arginfo_cmdbuf_createASEncoder,  ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry metal_compute_encoder_methods[] = {
    PHP_ME(Metal_ComputeCommandEncoder, setComputePipelineState,   arginfo_Metal_ComputeCommandEncoder_setComputePipelineState,   ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ComputeCommandEncoder, setBuffer,                 arginfo_Metal_ComputeCommandEncoder_setBuffer,                 ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ComputeCommandEncoder, setBytes,                  arginfo_Metal_ComputeCommandEncoder_setBytes,                  ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ComputeCommandEncoder, setTexture,                arginfo_Metal_Encoder_setTexture,                              ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ComputeCommandEncoder, setSamplerState,           arginfo_Metal_Encoder_setSamplerState,                         ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ComputeCommandEncoder, setThreadgroupMemoryLength,arginfo_Metal_ComputeCommandEncoder_setThreadgroupMemoryLength,ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ComputeCommandEncoder, dispatchThreads,           arginfo_Metal_ComputeCommandEncoder_dispatch,                  ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ComputeCommandEncoder, dispatchThreadgroups,      arginfo_Metal_ComputeCommandEncoder_dispatch,                  ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ComputeCommandEncoder, endEncoding,               arginfo_void,                                                  ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ComputeCommandEncoder, dispatchThreadsWithIndirectBuffer, arginfo_ce_indirectDispatch, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ComputeCommandEncoder, useResource,              arginfo_ce_useResource,  ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ComputeCommandEncoder, memoryBarrierWithScope,   arginfo_adv_void_1long,  ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ComputeCommandEncoder, updateFence,              arginfo_ce_fence,        ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ComputeCommandEncoder, waitForFence,             arginfo_ce_fence,        ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ComputeCommandEncoder, setAccelerationStructure, arginfo_ce_setAS,        ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry metal_render_encoder_methods[] = {
    PHP_ME(Metal_RenderCommandEncoder, setRenderPipelineState,  arginfo_Metal_RenderCommandEncoder_setRenderPipelineState,  ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, setVertexBuffer,         arginfo_Metal_RenderCommandEncoder_setVertexBuffer,         ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, setVertexBytes,          arginfo_Metal_ComputeCommandEncoder_setBytes,               ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, setFragmentBuffer,       arginfo_Metal_RenderCommandEncoder_setVertexBuffer,         ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, setFragmentBytes,        arginfo_Metal_ComputeCommandEncoder_setBytes,               ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, setFragmentTexture,      arginfo_Metal_Encoder_setTexture,                           ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, setFragmentSamplerState, arginfo_Metal_Encoder_setSamplerState,                      ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, setDepthStencilState,    arginfo_Metal_RenderCommandEncoder_setDepthStencilState,    ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, setViewport,             arginfo_Metal_RenderCommandEncoder_setViewport,             ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, setScissorRect,          arginfo_Metal_RenderCommandEncoder_setScissorRect,          ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, setCullMode,             arginfo_Metal_setLong,                                      ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, setFrontFacingWinding,   arginfo_Metal_setLong,                                      ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, setTriangleFillMode,     arginfo_Metal_setLong,                                      ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, drawPrimitives,          arginfo_Metal_RenderCommandEncoder_drawPrimitives,          ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, drawIndexedPrimitives,   arginfo_Metal_RenderCommandEncoder_drawIndexedPrimitives,   ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, endEncoding,             arginfo_void,                                               ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, setVertexTexture,                  arginfo_Metal_Encoder_setTexture,     ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, setVertexSamplerState,             arginfo_Metal_Encoder_setSamplerState,ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, drawPrimitivesInstanced,           arginfo_re_drawInstanced,             ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, drawIndexedPrimitivesInstanced,    arginfo_re_drawIdxInstanced,          ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, drawPrimitivesIndirect,            arginfo_re_drawIndirect,              ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, drawIndexedPrimitivesIndirect,     arginfo_re_drawIdxIndirect,           ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, setBlendColor,                     arginfo_re_blendColor,                ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, setStencilReferenceValue,          arginfo_adv_void_1long,               ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, setDepthBias,                      arginfo_re_depthBias,                 ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, updateFence,                       arginfo_re_fence,                     ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, waitForFence,                      arginfo_re_fence,                     ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderCommandEncoder, useResource,                       arginfo_re_useResource,               ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry metal_blit_encoder_methods[] = {
    PHP_ME(Metal_BlitCommandEncoder, copyFromBuffer,       arginfo_Metal_BlitCommandEncoder_copyFromBuffer,       ZEND_ACC_PUBLIC)
    PHP_ME(Metal_BlitCommandEncoder, synchronizeResource,  arginfo_Metal_BlitCommandEncoder_synchronizeResource,  ZEND_ACC_PUBLIC)
    PHP_ME(Metal_BlitCommandEncoder, endEncoding,          arginfo_void,                                          ZEND_ACC_PUBLIC)
    PHP_ME(Metal_BlitCommandEncoder, generateMipmaps,                arginfo_blit_genMips, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_BlitCommandEncoder, fillBuffer,                     arginfo_blit_fill,    ZEND_ACC_PUBLIC)
    PHP_ME(Metal_BlitCommandEncoder, copyFromTexture,                arginfo_blit_copyTex, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_BlitCommandEncoder, optimizeContentsForGPUAccess,   arginfo_blit_genMips, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_BlitCommandEncoder, optimizeContentsForCPUAccess,   arginfo_blit_genMips, ZEND_ACC_PUBLIC)
    /* buildAccelerationStructure requires AccelerationStructureCommandEncoder — future version */
    PHP_FE_END
};

static const zend_function_entry metal_library_methods[] = {
    PHP_ME(Metal_Library, getFunction,     arginfo_Metal_Library_getFunction,     ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Library, getFunctionNames, arginfo_Metal_Library_getFunctionNames, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry metal_function_methods[] = {
    PHP_ME(Metal_MetalFunction, getName,         arginfo_Metal_MetalFunction_getName,         ZEND_ACC_PUBLIC)
    PHP_ME(Metal_MetalFunction, getFunctionType,      arginfo_Metal_MetalFunction_getFunctionType, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_MetalFunction, createArgumentEncoder, arginfo_fn_createArgEnc,                    ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry metal_compute_pipeline_state_methods[] = {
    PHP_ME(Metal_ComputePipelineState, getMaxTotalThreadsPerThreadgroup, arginfo_Metal_ComputePipelineState_getMaxTotalThreadsPerThreadgroup, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ComputePipelineState, getThreadExecutionWidth,         arginfo_Metal_ComputePipelineState_getThreadExecutionWidth,         ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry metal_texture_methods[] = {
    PHP_ME(Metal_Texture, getWidth,       arginfo_Metal_Texture_getDimension, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Texture, getHeight,      arginfo_Metal_Texture_getDimension, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Texture, getDepth,       arginfo_Metal_Texture_getDimension, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Texture, getPixelFormat, arginfo_Metal_Texture_getDimension, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Texture, getTextureType, arginfo_Metal_Texture_getDimension, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Texture, replaceRegion,  arginfo_Metal_Texture_replaceRegion, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Texture, getBytes,            arginfo_Metal_Texture_getBytes,     ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Texture, newTextureView,      arginfo_tex_newView,                ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Texture, getUsage,            arginfo_adv_long,                   ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Texture, getStorageMode,      arginfo_adv_long,                   ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Texture, getMipmapLevelCount, arginfo_adv_long,                   ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Texture, getArrayLength,      arginfo_adv_long,                   ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Texture, getSampleCount,      arginfo_adv_long,                   ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Texture, isFramebufferOnly,   arginfo_adv_bool,                   ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Texture, makeAliasable,       arginfo_adv_void,                   ZEND_ACC_PUBLIC)
    PHP_ME(Metal_Texture, isAliasable,         arginfo_adv_bool,                   ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry metal_texture_descriptor_methods[] = {
    PHP_ME(Metal_TextureDescriptor, __construct,          arginfo_Metal_TextureDescriptor___construct,          ZEND_ACC_PUBLIC)
    PHP_ME(Metal_TextureDescriptor, texture2DDescriptor,  arginfo_Metal_TextureDescriptor_texture2DDescriptor,  ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(Metal_TextureDescriptor, setPixelFormat,       arginfo_Metal_setLong,                                ZEND_ACC_PUBLIC)
    PHP_ME(Metal_TextureDescriptor, setWidth,             arginfo_Metal_setLong,                                ZEND_ACC_PUBLIC)
    PHP_ME(Metal_TextureDescriptor, setHeight,            arginfo_Metal_setLong,                                ZEND_ACC_PUBLIC)
    PHP_ME(Metal_TextureDescriptor, setTextureType,       arginfo_Metal_setLong,                                ZEND_ACC_PUBLIC)
    PHP_ME(Metal_TextureDescriptor, setUsage,             arginfo_Metal_setLong,                                ZEND_ACC_PUBLIC)
    PHP_ME(Metal_TextureDescriptor, setStorageMode,       arginfo_Metal_setLong,                                ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry metal_render_pipeline_descriptor_methods[] = {
    PHP_ME(Metal_RenderPipelineDescriptor, __construct,                 arginfo_Metal_RenderPipelineDescriptor___construct,             ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderPipelineDescriptor, setVertexFunction,           arginfo_Metal_RenderPipelineDescriptor_setFunction,             ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderPipelineDescriptor, setFragmentFunction,         arginfo_Metal_RenderPipelineDescriptor_setFunction,             ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderPipelineDescriptor, getColorAttachment,          arginfo_Metal_RenderPipelineDescriptor_getColorAttachment,      ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderPipelineDescriptor, setDepthAttachmentPixelFormat, arginfo_Metal_setLong,                                       ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderPipelineDescriptor, setVertexDescriptor,         arginfo_Metal_RenderPipelineDescriptor_setVertexDescriptor,     ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry metal_render_pass_descriptor_methods[] = {
    PHP_ME(Metal_RenderPassDescriptor, __construct,                    arginfo_Metal_RenderPassDescriptor___construct,                    ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderPassDescriptor, setColorAttachmentTexture,      arginfo_Metal_RenderPassDescriptor_setColorAttachmentTexture,      ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderPassDescriptor, setColorAttachmentLoadAction,   arginfo_Metal_RenderPassDescriptor_setColorAttachmentAction,       ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderPassDescriptor, setColorAttachmentStoreAction,  arginfo_Metal_RenderPassDescriptor_setColorAttachmentAction,       ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderPassDescriptor, setColorAttachmentClearColor,   arginfo_Metal_RenderPassDescriptor_setColorAttachmentClearColor,   ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderPassDescriptor, setDepthAttachmentTexture,      arginfo_Metal_RenderPassDescriptor_setDepthAttachmentTexture,      ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderPassDescriptor, setDepthAttachmentLoadAction,   arginfo_Metal_setLong,                                            ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderPassDescriptor, setDepthAttachmentStoreAction,  arginfo_Metal_setLong,                                            ZEND_ACC_PUBLIC)
    PHP_ME(Metal_RenderPassDescriptor, setDepthAttachmentClearDepth,   arginfo_Metal_setBool,                                            ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry metal_color_attachment_descriptor_methods[] = {
    PHP_ME(Metal_ColorAttachmentDescriptor, setPixelFormat,                arginfo_Metal_setLong,  ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ColorAttachmentDescriptor, setBlendingEnabled,            arginfo_Metal_setBool,  ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ColorAttachmentDescriptor, setSourceRGBBlendFactor,       arginfo_Metal_setLong,  ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ColorAttachmentDescriptor, setDestinationRGBBlendFactor,  arginfo_Metal_setLong,  ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ColorAttachmentDescriptor, setSourceAlphaBlendFactor,     arginfo_Metal_setLong,  ZEND_ACC_PUBLIC)
    PHP_ME(Metal_ColorAttachmentDescriptor, setDestinationAlphaBlendFactor,arginfo_Metal_setLong,  ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry metal_depth_stencil_descriptor_methods[] = {
    PHP_ME(Metal_DepthStencilDescriptor, __construct,            arginfo_Metal_DepthStencilDescriptor___construct, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_DepthStencilDescriptor, setDepthCompareFunction,arginfo_Metal_setLong,                           ZEND_ACC_PUBLIC)
    PHP_ME(Metal_DepthStencilDescriptor, setDepthWriteEnabled,   arginfo_Metal_setBool,                           ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry metal_vertex_descriptor_methods[] = {
    PHP_ME(Metal_VertexDescriptor, __construct,  arginfo_Metal_VertexDescriptor___construct,  ZEND_ACC_PUBLIC)
    PHP_ME(Metal_VertexDescriptor, setAttribute, arginfo_Metal_VertexDescriptor_setAttribute, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_VertexDescriptor, setLayout,    arginfo_Metal_VertexDescriptor_setLayout,    ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry metal_sampler_descriptor_methods[] = {
    PHP_ME(Metal_SamplerDescriptor, __construct,    arginfo_Metal_SamplerDescriptor___construct, ZEND_ACC_PUBLIC)
    PHP_ME(Metal_SamplerDescriptor, setMinFilter,   arginfo_Metal_setLong,                      ZEND_ACC_PUBLIC)
    PHP_ME(Metal_SamplerDescriptor, setMagFilter,   arginfo_Metal_setLong,                      ZEND_ACC_PUBLIC)
    PHP_ME(Metal_SamplerDescriptor, setSAddressMode, arginfo_Metal_setLong,                     ZEND_ACC_PUBLIC)
    PHP_ME(Metal_SamplerDescriptor, setTAddressMode, arginfo_Metal_setLong,                     ZEND_ACC_PUBLIC)
    PHP_FE_END
};

/* Empty method tables for types that are created internally only */
static const zend_function_entry metal_render_pipeline_state_methods[] = { PHP_FE_END };
static const zend_function_entry metal_drawable_methods[]              = { PHP_FE_END };
static const zend_function_entry metal_sampler_state_methods[]         = { PHP_FE_END };
static const zend_function_entry metal_depth_stencil_state_methods[]   = { PHP_FE_END };

/* ====================================================================
 *  Namespace functions
 * ==================================================================== */

static const zend_function_entry metal_functions[] = {
    ZEND_NS_NAMED_FE("Metal", createSystemDefaultDevice, ZEND_FN(Metal_createSystemDefaultDevice), arginfo_Metal_createSystemDefaultDevice)
    ZEND_NS_NAMED_FE("Metal", allDevices,                ZEND_FN(Metal_allDevices),                arginfo_Metal_allDevices)
    PHP_FE_END
};

/* METAL_REGISTER_CLASS macro is in metal_internal.h */

/* ====================================================================
 *  MINIT — register all classes and constants
 * ==================================================================== */

PHP_MINIT_FUNCTION(metal)
{
    /* Exception class */
    zend_class_entry tmp_ce;
    INIT_NS_CLASS_ENTRY(tmp_ce, "Metal", "Exception", NULL);
    metal_ce_exception = zend_register_internal_class_ex(&tmp_ce, zend_ce_exception);

    /* Core classes */
    METAL_REGISTER_CLASS(device,                  "Device",                    metal_device_methods,                    metal_device_handlers,                    metal_ce_device,                    ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(buffer,                  "Buffer",                    metal_buffer_methods,                    metal_buffer_handlers,                    metal_ce_buffer,                    ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(command_queue,            "CommandQueue",              metal_command_queue_methods,              metal_command_queue_handlers,              metal_ce_command_queue,              ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(command_buffer,           "CommandBuffer",             metal_command_buffer_methods,             metal_command_buffer_handlers,             metal_ce_command_buffer,             ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(compute_encoder,          "ComputeCommandEncoder",     metal_compute_encoder_methods,            metal_compute_encoder_handlers,            metal_ce_compute_encoder,            ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(render_encoder,           "RenderCommandEncoder",      metal_render_encoder_methods,             metal_render_encoder_handlers,             metal_ce_render_encoder,             ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(library,                  "Library",                   metal_library_methods,                    metal_library_handlers,                    metal_ce_library,                    ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(function,                 "MetalFunction",             metal_function_methods,                   metal_function_handlers,                   metal_ce_metal_function,             ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(compute_pipeline_state,   "ComputePipelineState",      metal_compute_pipeline_state_methods,     metal_compute_pipeline_state_handlers,     metal_ce_compute_pipeline_state,     ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(render_pipeline_state,    "RenderPipelineState",       metal_render_pipeline_state_methods,      metal_render_pipeline_state_handlers,      metal_ce_render_pipeline_state,      ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(texture,                  "Texture",                   metal_texture_methods,                    metal_texture_handlers,                    metal_ce_texture,                    ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(texture_descriptor,       "TextureDescriptor",         metal_texture_descriptor_methods,         metal_texture_descriptor_handlers,         metal_ce_texture_descriptor,         ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(render_pipeline_descriptor,"RenderPipelineDescriptor", metal_render_pipeline_descriptor_methods, metal_render_pipeline_descriptor_handlers, metal_ce_render_pipeline_descriptor, ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(render_pass_descriptor,   "RenderPassDescriptor",      metal_render_pass_descriptor_methods,     metal_render_pass_descriptor_handlers,     metal_ce_render_pass_descriptor,     ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(color_attachment_descriptor,"ColorAttachmentDescriptor",metal_color_attachment_descriptor_methods,metal_color_attachment_descriptor_handlers,metal_ce_color_attachment_descriptor, ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(depth_stencil_state,      "DepthStencilState",         metal_depth_stencil_state_methods,        metal_depth_stencil_state_handlers,        metal_ce_depth_stencil_state,        ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(depth_stencil_descriptor, "DepthStencilDescriptor",    metal_depth_stencil_descriptor_methods,   metal_depth_stencil_descriptor_handlers,   metal_ce_depth_stencil_descriptor,   ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(vertex_descriptor,        "VertexDescriptor",          metal_vertex_descriptor_methods,          metal_vertex_descriptor_handlers,          metal_ce_vertex_descriptor,          ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(vertex_buffer_layout_descriptor, "VertexBufferLayoutDescriptor", NULL, metal_vertex_buffer_layout_descriptor_handlers, metal_ce_vertex_buffer_layout_descriptor, ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(vertex_attribute_descriptor, "VertexAttributeDescriptor", NULL, metal_vertex_attribute_descriptor_handlers, metal_ce_vertex_attribute_descriptor, ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(sampler_state,            "SamplerState",              metal_sampler_state_methods,              metal_sampler_state_handlers,              metal_ce_sampler_state,              ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(sampler_descriptor,       "SamplerDescriptor",         metal_sampler_descriptor_methods,         metal_sampler_descriptor_handlers,         metal_ce_sampler_descriptor,         ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(blit_encoder,             "BlitCommandEncoder",        metal_blit_encoder_methods,               metal_blit_encoder_handlers,               metal_ce_blit_encoder,               ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);
    METAL_REGISTER_CLASS(drawable,                 "Drawable",                  metal_drawable_methods,                   metal_drawable_handlers,                   metal_ce_drawable,                   ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES);

    /* ----------------------------------------------------------------
     *  Constants — Metal\FLOAT, Metal\INT32, ...
     * ---------------------------------------------------------------- */

    /* Data types for buffer read/write */
    REGISTER_NS_LONG_CONSTANT("Metal", "FLOAT",   METAL_DTYPE_FLOAT,   CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "INT32",   METAL_DTYPE_INT32,   CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "UINT32",  METAL_DTYPE_UINT32,  CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "INT16",   METAL_DTYPE_INT16,   CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "UINT16",  METAL_DTYPE_UINT16,  CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "INT8",    METAL_DTYPE_INT8,    CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "UINT8",   METAL_DTYPE_UINT8,   CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "FLOAT16", METAL_DTYPE_FLOAT16, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "DOUBLE",  METAL_DTYPE_DOUBLE,  CONST_CS | CONST_PERSISTENT);

    /* Resource storage modes */
    REGISTER_NS_LONG_CONSTANT("Metal", "StorageModeShared",   MTLStorageModeShared,   CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "StorageModeManaged",  MTLStorageModeManaged,  CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "StorageModePrivate",  MTLStorageModePrivate,  CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "StorageModeMemoryless",MTLStorageModeMemoryless,CONST_CS | CONST_PERSISTENT);

    /* Command buffer status */
    REGISTER_NS_LONG_CONSTANT("Metal", "CommandBufferStatusNotEnqueued", MTLCommandBufferStatusNotEnqueued, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "CommandBufferStatusEnqueued",    MTLCommandBufferStatusEnqueued,    CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "CommandBufferStatusCommitted",   MTLCommandBufferStatusCommitted,   CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "CommandBufferStatusScheduled",   MTLCommandBufferStatusScheduled,   CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "CommandBufferStatusCompleted",   MTLCommandBufferStatusCompleted,   CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "CommandBufferStatusError",       MTLCommandBufferStatusError,       CONST_CS | CONST_PERSISTENT);

    /* Pixel formats (most common) */
    REGISTER_NS_LONG_CONSTANT("Metal", "PixelFormatInvalid",        MTLPixelFormatInvalid,        CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "PixelFormatA8Unorm",        MTLPixelFormatA8Unorm,        CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "PixelFormatR8Unorm",        MTLPixelFormatR8Unorm,        CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "PixelFormatR16Float",       MTLPixelFormatR16Float,       CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "PixelFormatR32Float",       MTLPixelFormatR32Float,       CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "PixelFormatRG8Unorm",       MTLPixelFormatRG8Unorm,       CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "PixelFormatRG16Float",      MTLPixelFormatRG16Float,      CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "PixelFormatRG32Float",      MTLPixelFormatRG32Float,      CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "PixelFormatRGBA8Unorm",     MTLPixelFormatRGBA8Unorm,     CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "PixelFormatRGBA8Unorm_sRGB",MTLPixelFormatRGBA8Unorm_sRGB,CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "PixelFormatBGRA8Unorm",     MTLPixelFormatBGRA8Unorm,     CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "PixelFormatBGRA8Unorm_sRGB",MTLPixelFormatBGRA8Unorm_sRGB,CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "PixelFormatRGBA16Float",    MTLPixelFormatRGBA16Float,    CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "PixelFormatRGBA32Float",    MTLPixelFormatRGBA32Float,    CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "PixelFormatDepth32Float",   MTLPixelFormatDepth32Float,   CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "PixelFormatDepth16Unorm",   MTLPixelFormatDepth16Unorm,   CONST_CS | CONST_PERSISTENT);

    /* Primitive types */
    REGISTER_NS_LONG_CONSTANT("Metal", "PrimitiveTypePoint",         MTLPrimitiveTypePoint,         CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "PrimitiveTypeLine",          MTLPrimitiveTypeLine,          CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "PrimitiveTypeLineStrip",     MTLPrimitiveTypeLineStrip,     CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "PrimitiveTypeTriangle",      MTLPrimitiveTypeTriangle,      CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "PrimitiveTypeTriangleStrip", MTLPrimitiveTypeTriangleStrip, CONST_CS | CONST_PERSISTENT);

    /* Index types */
    REGISTER_NS_LONG_CONSTANT("Metal", "IndexTypeUInt16", MTLIndexTypeUInt16, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "IndexTypeUInt32", MTLIndexTypeUInt32, CONST_CS | CONST_PERSISTENT);

    /* Cull modes */
    REGISTER_NS_LONG_CONSTANT("Metal", "CullModeNone",  MTLCullModeNone,  CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "CullModeFront", MTLCullModeFront, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "CullModeBack",  MTLCullModeBack,  CONST_CS | CONST_PERSISTENT);

    /* Winding */
    REGISTER_NS_LONG_CONSTANT("Metal", "WindingClockwise",        MTLWindingClockwise,        CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "WindingCounterClockwise", MTLWindingCounterClockwise, CONST_CS | CONST_PERSISTENT);

    /* Triangle fill mode */
    REGISTER_NS_LONG_CONSTANT("Metal", "TriangleFillModeFill",  MTLTriangleFillModeFill,  CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "TriangleFillModeLines", MTLTriangleFillModeLines, CONST_CS | CONST_PERSISTENT);

    /* Load / store actions */
    REGISTER_NS_LONG_CONSTANT("Metal", "LoadActionDontCare", MTLLoadActionDontCare, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "LoadActionLoad",     MTLLoadActionLoad,     CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "LoadActionClear",    MTLLoadActionClear,    CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "StoreActionDontCare",MTLStoreActionDontCare,CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "StoreActionStore",   MTLStoreActionStore,   CONST_CS | CONST_PERSISTENT);

    /* Compare functions (depth/stencil) */
    REGISTER_NS_LONG_CONSTANT("Metal", "CompareFunctionNever",        MTLCompareFunctionNever,        CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "CompareFunctionLess",         MTLCompareFunctionLess,         CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "CompareFunctionEqual",        MTLCompareFunctionEqual,        CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "CompareFunctionLessEqual",    MTLCompareFunctionLessEqual,    CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "CompareFunctionGreater",      MTLCompareFunctionGreater,      CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "CompareFunctionNotEqual",     MTLCompareFunctionNotEqual,     CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "CompareFunctionGreaterEqual", MTLCompareFunctionGreaterEqual, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "CompareFunctionAlways",       MTLCompareFunctionAlways,       CONST_CS | CONST_PERSISTENT);

    /* Blend factors */
    REGISTER_NS_LONG_CONSTANT("Metal", "BlendFactorZero",                  MTLBlendFactorZero,                  CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "BlendFactorOne",                   MTLBlendFactorOne,                   CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "BlendFactorSourceAlpha",           MTLBlendFactorSourceAlpha,           CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "BlendFactorOneMinusSourceAlpha",   MTLBlendFactorOneMinusSourceAlpha,   CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "BlendFactorDestinationAlpha",      MTLBlendFactorDestinationAlpha,      CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "BlendFactorOneMinusDestinationAlpha", MTLBlendFactorOneMinusDestinationAlpha, CONST_CS | CONST_PERSISTENT);

    /* Texture types */
    REGISTER_NS_LONG_CONSTANT("Metal", "TextureType1D",        MTLTextureType1D,        CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "TextureType2D",        MTLTextureType2D,        CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "TextureType2DMultisample", MTLTextureType2DMultisample, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "TextureTypeCube",      MTLTextureTypeCube,      CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "TextureType3D",        MTLTextureType3D,        CONST_CS | CONST_PERSISTENT);

    /* Texture usage */
    REGISTER_NS_LONG_CONSTANT("Metal", "TextureUsageUnknown",        MTLTextureUsageUnknown,        CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "TextureUsageShaderRead",     MTLTextureUsageShaderRead,     CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "TextureUsageShaderWrite",    MTLTextureUsageShaderWrite,    CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "TextureUsageRenderTarget",   MTLTextureUsageRenderTarget,   CONST_CS | CONST_PERSISTENT);

    /* Sampler filter */
    REGISTER_NS_LONG_CONSTANT("Metal", "SamplerMinMagFilterNearest", MTLSamplerMinMagFilterNearest, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "SamplerMinMagFilterLinear",  MTLSamplerMinMagFilterLinear,  CONST_CS | CONST_PERSISTENT);

    /* Sampler address mode */
    REGISTER_NS_LONG_CONSTANT("Metal", "SamplerAddressModeClampToEdge",    MTLSamplerAddressModeClampToEdge,    CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "SamplerAddressModeMirrorClampToEdge", MTLSamplerAddressModeMirrorClampToEdge, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "SamplerAddressModeRepeat",         MTLSamplerAddressModeRepeat,         CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "SamplerAddressModeMirrorRepeat",   MTLSamplerAddressModeMirrorRepeat,   CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "SamplerAddressModeClampToZero",    MTLSamplerAddressModeClampToZero,    CONST_CS | CONST_PERSISTENT);

    /* Vertex formats (most common) */
    REGISTER_NS_LONG_CONSTANT("Metal", "VertexFormatFloat",  MTLVertexFormatFloat,  CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "VertexFormatFloat2", MTLVertexFormatFloat2, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "VertexFormatFloat3", MTLVertexFormatFloat3, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "VertexFormatFloat4", MTLVertexFormatFloat4, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "VertexFormatInt",    MTLVertexFormatInt,    CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "VertexFormatInt2",   MTLVertexFormatInt2,   CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "VertexFormatInt3",   MTLVertexFormatInt3,   CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "VertexFormatInt4",   MTLVertexFormatInt4,   CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "VertexFormatUChar4Normalized", MTLVertexFormatUChar4Normalized, CONST_CS | CONST_PERSISTENT);

    /* Vertex step function */
    REGISTER_NS_LONG_CONSTANT("Metal", "VertexStepFunctionConstant",  MTLVertexStepFunctionConstant,  CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "VertexStepFunctionPerVertex", MTLVertexStepFunctionPerVertex, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "VertexStepFunctionPerInstance", MTLVertexStepFunctionPerInstance, CONST_CS | CONST_PERSISTENT);

    /* Function types */
    REGISTER_NS_LONG_CONSTANT("Metal", "FunctionTypeVertex",   MTLFunctionTypeVertex,   CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "FunctionTypeFragment", MTLFunctionTypeFragment, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "FunctionTypeKernel",   MTLFunctionTypeKernel,   CONST_CS | CONST_PERSISTENT);

    /* GPU families (for supportsFamily) */
    REGISTER_NS_LONG_CONSTANT("Metal", "GPUFamilyApple1", MTLGPUFamilyApple1, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "GPUFamilyApple2", MTLGPUFamilyApple2, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "GPUFamilyApple3", MTLGPUFamilyApple3, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "GPUFamilyApple4", MTLGPUFamilyApple4, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "GPUFamilyApple5", MTLGPUFamilyApple5, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "GPUFamilyApple6", MTLGPUFamilyApple6, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "GPUFamilyApple7", MTLGPUFamilyApple7, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "GPUFamilyApple8", MTLGPUFamilyApple8, CONST_CS | CONST_PERSISTENT);
#if defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && __MAC_OS_X_VERSION_MAX_ALLOWED >= 140000
    REGISTER_NS_LONG_CONSTANT("Metal", "GPUFamilyApple9", MTLGPUFamilyApple9, CONST_CS | CONST_PERSISTENT);
#endif
    REGISTER_NS_LONG_CONSTANT("Metal", "GPUFamilyCommon1", MTLGPUFamilyCommon1, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "GPUFamilyCommon2", MTLGPUFamilyCommon2, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "GPUFamilyCommon3", MTLGPUFamilyCommon3, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Metal", "GPUFamilyMetal3",  MTLGPUFamilyMetal3,  CONST_CS | CONST_PERSISTENT);

    /* Register advanced classes (from metal_advanced.c) */
    metal_register_event_classes();
    metal_register_heap_classes();
    metal_register_indirect_classes();
    metal_register_argument_encoder_class();
    metal_register_acceleration_classes();
    metal_register_binary_archive_classes();
    metal_register_compute_pass_descriptor_class();
    metal_register_accel_encoder_class();
    metal_register_indirect_command_classes();
    metal_register_mesh_shader_classes();
    metal_register_advanced_constants(module_number);

    return SUCCESS;
}

/* ====================================================================
 *  Module info
 * ==================================================================== */

PHP_MINFO_FUNCTION(metal)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "Metal GPU Support", "enabled");
    php_info_print_table_row(2, "Version", PHP_METAL_VERSION);

    /* Show GPU info */
    @autoreleasepool {
        id<MTLDevice> dev = MTLCreateSystemDefaultDevice();
        if (dev) {
            php_info_print_table_row(2, "GPU", [[dev name] UTF8String]);

            char family[64];
#if defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && __MAC_OS_X_VERSION_MAX_ALLOWED >= 140000
            if ([dev supportsFamily:MTLGPUFamilyApple9]) snprintf(family, sizeof(family), "Apple9+");
            else
#endif
            if ([dev supportsFamily:MTLGPUFamilyApple8]) snprintf(family, sizeof(family), "Apple8");
            else if ([dev supportsFamily:MTLGPUFamilyApple7]) snprintf(family, sizeof(family), "Apple7");
            else if ([dev supportsFamily:MTLGPUFamilyApple6]) snprintf(family, sizeof(family), "Apple6");
            else if ([dev supportsFamily:MTLGPUFamilyApple5]) snprintf(family, sizeof(family), "Apple5");
            else snprintf(family, sizeof(family), "Apple4 or earlier");
            php_info_print_table_row(2, "GPU Family", family);

            char max_buf[64];
            snprintf(max_buf, sizeof(max_buf), "%lu MB", (unsigned long)([dev maxBufferLength] / 1024 / 1024));
            php_info_print_table_row(2, "Max Buffer Size", max_buf);

            char max_tg[64];
            snprintf(max_tg, sizeof(max_tg), "%lu bytes", (unsigned long)[dev maxThreadgroupMemoryLength]);
            php_info_print_table_row(2, "Max Threadgroup Memory", max_tg);
        } else {
            php_info_print_table_row(2, "GPU", "Not available");
        }
    }

    php_info_print_table_end();
}

/* ====================================================================
 *  Module entry
 * ==================================================================== */

zend_module_entry metal_module_entry = {
    STANDARD_MODULE_HEADER,
    "metal",
    metal_functions,
    PHP_MINIT(metal),
    NULL,  /* MSHUTDOWN */
    NULL,  /* RINIT */
    NULL,  /* RSHUTDOWN */
    PHP_MINFO(metal),
    PHP_METAL_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_METAL
ZEND_GET_MODULE(metal)
#endif
