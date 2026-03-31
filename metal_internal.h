/*
  +----------------------------------------------------------------------+
  | php-metal — Shared internal header                                   |
  +----------------------------------------------------------------------+
*/

#ifndef METAL_INTERNAL_H
#define METAL_INTERNAL_H

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend_exceptions.h"
#include "php_metal.h"

#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>
#import <Foundation/Foundation.h>
#import <AppKit/NSWindow.h>
#import <AppKit/NSView.h>

/* ====================================================================
 *  Data type constants for buffer read/write
 * ==================================================================== */

#define METAL_DTYPE_FLOAT   0
#define METAL_DTYPE_INT32   1
#define METAL_DTYPE_UINT32  2
#define METAL_DTYPE_INT16   3
#define METAL_DTYPE_UINT16  4
#define METAL_DTYPE_INT8    5
#define METAL_DTYPE_UINT8   6
#define METAL_DTYPE_FLOAT16 7
#define METAL_DTYPE_DOUBLE  8

/* ====================================================================
 *  Helper macro — fetch internal struct from zend_object
 * ==================================================================== */

#define METAL_OBJ(type, obj) \
    ((type *)((char *)(obj) - XtOffsetOf(type, std)))

/* ====================================================================
 *  Helper macro — wrap ObjC object in PHP return value
 * ==================================================================== */

#define METAL_WRAP_RETURN(short_name, type_t, field, ce_ptr, objc_obj)     \
do {                                                                       \
    object_init_ex(return_value, ce_ptr);                                  \
    type_t *_intern = metal_##short_name##_from_obj(Z_OBJ_P(return_value));\
    _intern->field = objc_obj;                                             \
} while (0)

/* ====================================================================
 *  Generic create / free macro
 * ==================================================================== */

#define METAL_DEFINE_CREATE_FREE(short_name, type_t, field, ce_ptr, handlers_ptr) \
static zend_object *metal_##short_name##_create(zend_class_entry *ce) {           \
    type_t *intern = zend_object_alloc(sizeof(type_t), ce);                       \
    intern->field = nil;                                                          \
    zend_object_std_init(&intern->std, ce);                                       \
    object_properties_init(&intern->std, ce);                                     \
    intern->std.handlers = handlers_ptr;                                          \
    return &intern->std;                                                          \
}                                                                                 \
static void metal_##short_name##_free(zend_object *obj) {                         \
    type_t *intern = metal_##short_name##_from_obj(obj);                          \
    intern->field = nil;                                                          \
    zend_object_std_dtor(&intern->std);                                           \
}

/* ====================================================================
 *  Class registration macro
 * ==================================================================== */

#define METAL_REGISTER_CLASS(short_name, ns_name, methods, handlers_var, ce_var, flags) \
do {                                                                                    \
    zend_class_entry tmp_ce;                                                            \
    INIT_NS_CLASS_ENTRY(tmp_ce, "Metal", ns_name, methods);                             \
    ce_var = zend_register_internal_class(&tmp_ce);                                     \
    ce_var->ce_flags |= (flags);                                                        \
    ce_var->create_object = metal_##short_name##_create;                                \
    memcpy(&handlers_var, zend_get_std_object_handlers(), sizeof(zend_object_handlers));\
    handlers_var.offset   = XtOffsetOf(metal_##short_name##_t, std);                    \
    handlers_var.free_obj = metal_##short_name##_free;                                  \
    handlers_var.clone_obj = NULL;                                                      \
} while (0)

/* ====================================================================
 *  Core object structs
 * ==================================================================== */

typedef struct { id<MTLDevice> device;                                        zend_object std; } metal_device_t;
typedef struct { id<MTLBuffer> buffer;                                        zend_object std; } metal_buffer_t;
typedef struct { id<MTLCommandQueue> queue;                                   zend_object std; } metal_command_queue_t;
typedef struct { id<MTLCommandBuffer> buffer;                                 zend_object std; } metal_command_buffer_t;
typedef struct { id<MTLComputeCommandEncoder> encoder;                        zend_object std; } metal_compute_encoder_t;
typedef struct { id<MTLLibrary> library;                                      zend_object std; } metal_library_t;
typedef struct { id<MTLFunction> function;                                    zend_object std; } metal_function_t;
typedef struct { id<MTLComputePipelineState> state;                           zend_object std; } metal_compute_pipeline_state_t;
typedef struct { id<MTLRenderPipelineState> state;                            zend_object std; } metal_render_pipeline_state_t;
typedef struct { id<MTLRenderCommandEncoder> encoder;                         zend_object std; } metal_render_encoder_t;
typedef struct { id<MTLTexture> texture;                                      zend_object std; } metal_texture_t;
typedef struct { MTLTextureDescriptor *descriptor;                            zend_object std; } metal_texture_descriptor_t;
typedef struct { MTLRenderPipelineDescriptor *descriptor;                     zend_object std; } metal_render_pipeline_descriptor_t;
typedef struct { MTLRenderPassDescriptor *descriptor;                         zend_object std; } metal_render_pass_descriptor_t;
typedef struct { MTLRenderPipelineColorAttachmentDescriptor *descriptor;      zend_object std; } metal_color_attachment_descriptor_t;
typedef struct { id<MTLDepthStencilState> state;                              zend_object std; } metal_depth_stencil_state_t;
typedef struct { MTLDepthStencilDescriptor *descriptor;                       zend_object std; } metal_depth_stencil_descriptor_t;
typedef struct { MTLVertexDescriptor *descriptor;                             zend_object std; } metal_vertex_descriptor_t;
typedef struct { MTLVertexBufferLayoutDescriptor *descriptor;                 zend_object std; } metal_vertex_buffer_layout_descriptor_t;
typedef struct { MTLVertexAttributeDescriptor *descriptor;                    zend_object std; } metal_vertex_attribute_descriptor_t;
typedef struct { id<MTLSamplerState> state;                                   zend_object std; } metal_sampler_state_t;
typedef struct { MTLSamplerDescriptor *descriptor;                            zend_object std; } metal_sampler_descriptor_t;
typedef struct { id<MTLBlitCommandEncoder> encoder;                           zend_object std; } metal_blit_encoder_t;
typedef struct { id<CAMetalDrawable> drawable;                                zend_object std; } metal_drawable_t;
typedef struct { CAMetalLayer *layer;                                         zend_object std; } metal_layer_t;

/* ====================================================================
 *  Advanced object structs
 * ==================================================================== */

typedef struct { id<MTLEvent> event;                                          zend_object std; } metal_event_t;
typedef struct { id<MTLSharedEvent> event;                                    zend_object std; } metal_shared_event_t;
typedef struct { id<MTLFence> fence;                                          zend_object std; } metal_fence_t;
typedef struct { id<MTLHeap> heap;                                            zend_object std; } metal_heap_t;
typedef struct { MTLHeapDescriptor *descriptor;                               zend_object std; } metal_heap_descriptor_t;
typedef struct { id<MTLIndirectCommandBuffer> buffer;                         zend_object std; } metal_indirect_command_buffer_t;
typedef struct { MTLIndirectCommandBufferDescriptor *descriptor;              zend_object std; } metal_icb_descriptor_t;
typedef struct { id<MTLArgumentEncoder> encoder;                              zend_object std; } metal_argument_encoder_t;
typedef struct { id<MTLAccelerationStructure> accel;                          zend_object std; } metal_acceleration_structure_t;
typedef struct { MTLPrimitiveAccelerationStructureDescriptor *descriptor;     zend_object std; } metal_prim_accel_descriptor_t;
typedef struct { MTLInstanceAccelerationStructureDescriptor *descriptor;      zend_object std; } metal_inst_accel_descriptor_t;
typedef struct { MTLAccelerationStructureTriangleGeometryDescriptor *descriptor; zend_object std; } metal_triangle_geometry_descriptor_t;
typedef struct { MTLAccelerationStructureBoundingBoxGeometryDescriptor *descriptor; zend_object std; } metal_bbox_geometry_descriptor_t;
typedef struct { id<MTLBinaryArchive> archive;                                zend_object std; } metal_binary_archive_t;
typedef struct { MTLBinaryArchiveDescriptor *descriptor;                      zend_object std; } metal_binary_archive_descriptor_t;
typedef struct { MTLComputePassDescriptor *descriptor;                        zend_object std; } metal_compute_pass_descriptor_t;
typedef struct { id<MTLAccelerationStructureCommandEncoder> encoder;           zend_object std; } metal_accel_encoder_t;
typedef struct { id<MTLIndirectRenderCommand> command;                         zend_object std; } metal_indirect_render_command_t;
typedef struct { id<MTLIndirectComputeCommand> command;                        zend_object std; } metal_indirect_compute_command_t;
typedef struct { MTLMeshRenderPipelineDescriptor *descriptor;                  zend_object std; } metal_mesh_render_pipeline_descriptor_t;
typedef struct { MTLStencilDescriptor *descriptor;                             zend_object std; } metal_stencil_descriptor_t;
typedef struct { MTLComputePipelineDescriptor *descriptor;                     zend_object std; } metal_compute_pipeline_descriptor_t;

/* ====================================================================
 *  Core from_obj inline accessors
 * ==================================================================== */

static inline metal_device_t *metal_device_from_obj(zend_object *o) { return METAL_OBJ(metal_device_t, o); }
static inline metal_buffer_t *metal_buffer_from_obj(zend_object *o) { return METAL_OBJ(metal_buffer_t, o); }
static inline metal_command_queue_t *metal_command_queue_from_obj(zend_object *o) { return METAL_OBJ(metal_command_queue_t, o); }
static inline metal_command_buffer_t *metal_command_buffer_from_obj(zend_object *o) { return METAL_OBJ(metal_command_buffer_t, o); }
static inline metal_compute_encoder_t *metal_compute_encoder_from_obj(zend_object *o) { return METAL_OBJ(metal_compute_encoder_t, o); }
static inline metal_library_t *metal_library_from_obj(zend_object *o) { return METAL_OBJ(metal_library_t, o); }
static inline metal_function_t *metal_function_from_obj(zend_object *o) { return METAL_OBJ(metal_function_t, o); }
static inline metal_compute_pipeline_state_t *metal_compute_pipeline_state_from_obj(zend_object *o) { return METAL_OBJ(metal_compute_pipeline_state_t, o); }
static inline metal_render_pipeline_state_t *metal_render_pipeline_state_from_obj(zend_object *o) { return METAL_OBJ(metal_render_pipeline_state_t, o); }
static inline metal_render_encoder_t *metal_render_encoder_from_obj(zend_object *o) { return METAL_OBJ(metal_render_encoder_t, o); }
static inline metal_texture_t *metal_texture_from_obj(zend_object *o) { return METAL_OBJ(metal_texture_t, o); }
static inline metal_texture_descriptor_t *metal_texture_descriptor_from_obj(zend_object *o) { return METAL_OBJ(metal_texture_descriptor_t, o); }
static inline metal_render_pipeline_descriptor_t *metal_render_pipeline_descriptor_from_obj(zend_object *o) { return METAL_OBJ(metal_render_pipeline_descriptor_t, o); }
static inline metal_render_pass_descriptor_t *metal_render_pass_descriptor_from_obj(zend_object *o) { return METAL_OBJ(metal_render_pass_descriptor_t, o); }
static inline metal_color_attachment_descriptor_t *metal_color_attachment_descriptor_from_obj(zend_object *o) { return METAL_OBJ(metal_color_attachment_descriptor_t, o); }
static inline metal_depth_stencil_state_t *metal_depth_stencil_state_from_obj(zend_object *o) { return METAL_OBJ(metal_depth_stencil_state_t, o); }
static inline metal_depth_stencil_descriptor_t *metal_depth_stencil_descriptor_from_obj(zend_object *o) { return METAL_OBJ(metal_depth_stencil_descriptor_t, o); }
static inline metal_vertex_descriptor_t *metal_vertex_descriptor_from_obj(zend_object *o) { return METAL_OBJ(metal_vertex_descriptor_t, o); }
static inline metal_vertex_buffer_layout_descriptor_t *metal_vertex_buffer_layout_descriptor_from_obj(zend_object *o) { return METAL_OBJ(metal_vertex_buffer_layout_descriptor_t, o); }
static inline metal_vertex_attribute_descriptor_t *metal_vertex_attribute_descriptor_from_obj(zend_object *o) { return METAL_OBJ(metal_vertex_attribute_descriptor_t, o); }
static inline metal_sampler_state_t *metal_sampler_state_from_obj(zend_object *o) { return METAL_OBJ(metal_sampler_state_t, o); }
static inline metal_sampler_descriptor_t *metal_sampler_descriptor_from_obj(zend_object *o) { return METAL_OBJ(metal_sampler_descriptor_t, o); }
static inline metal_blit_encoder_t *metal_blit_encoder_from_obj(zend_object *o) { return METAL_OBJ(metal_blit_encoder_t, o); }
static inline metal_drawable_t *metal_drawable_from_obj(zend_object *o) { return METAL_OBJ(metal_drawable_t, o); }
static inline metal_layer_t    *metal_layer_from_obj(zend_object *o)    { return METAL_OBJ(metal_layer_t, o); }

/* Advanced from_obj inline accessors */
static inline metal_event_t *metal_event_from_obj(zend_object *o) { return METAL_OBJ(metal_event_t, o); }
static inline metal_shared_event_t *metal_shared_event_from_obj(zend_object *o) { return METAL_OBJ(metal_shared_event_t, o); }
static inline metal_fence_t *metal_fence_from_obj(zend_object *o) { return METAL_OBJ(metal_fence_t, o); }
static inline metal_heap_t *metal_heap_from_obj(zend_object *o) { return METAL_OBJ(metal_heap_t, o); }
static inline metal_heap_descriptor_t *metal_heap_descriptor_from_obj(zend_object *o) { return METAL_OBJ(metal_heap_descriptor_t, o); }
static inline metal_indirect_command_buffer_t *metal_indirect_command_buffer_from_obj(zend_object *o) { return METAL_OBJ(metal_indirect_command_buffer_t, o); }
static inline metal_icb_descriptor_t *metal_icb_descriptor_from_obj(zend_object *o) { return METAL_OBJ(metal_icb_descriptor_t, o); }
static inline metal_argument_encoder_t *metal_argument_encoder_from_obj(zend_object *o) { return METAL_OBJ(metal_argument_encoder_t, o); }
static inline metal_acceleration_structure_t *metal_acceleration_structure_from_obj(zend_object *o) { return METAL_OBJ(metal_acceleration_structure_t, o); }
static inline metal_prim_accel_descriptor_t *metal_prim_accel_descriptor_from_obj(zend_object *o) { return METAL_OBJ(metal_prim_accel_descriptor_t, o); }
static inline metal_inst_accel_descriptor_t *metal_inst_accel_descriptor_from_obj(zend_object *o) { return METAL_OBJ(metal_inst_accel_descriptor_t, o); }
static inline metal_triangle_geometry_descriptor_t *metal_triangle_geometry_descriptor_from_obj(zend_object *o) { return METAL_OBJ(metal_triangle_geometry_descriptor_t, o); }
static inline metal_bbox_geometry_descriptor_t *metal_bbox_geometry_descriptor_from_obj(zend_object *o) { return METAL_OBJ(metal_bbox_geometry_descriptor_t, o); }
static inline metal_binary_archive_t *metal_binary_archive_from_obj(zend_object *o) { return METAL_OBJ(metal_binary_archive_t, o); }
static inline metal_binary_archive_descriptor_t *metal_binary_archive_descriptor_from_obj(zend_object *o) { return METAL_OBJ(metal_binary_archive_descriptor_t, o); }
static inline metal_compute_pass_descriptor_t *metal_compute_pass_descriptor_from_obj(zend_object *o) { return METAL_OBJ(metal_compute_pass_descriptor_t, o); }
static inline metal_accel_encoder_t *metal_accel_encoder_from_obj(zend_object *o) { return METAL_OBJ(metal_accel_encoder_t, o); }
static inline metal_indirect_render_command_t *metal_indirect_render_command_from_obj(zend_object *o) { return METAL_OBJ(metal_indirect_render_command_t, o); }
static inline metal_indirect_compute_command_t *metal_indirect_compute_command_from_obj(zend_object *o) { return METAL_OBJ(metal_indirect_compute_command_t, o); }
static inline metal_mesh_render_pipeline_descriptor_t *metal_mesh_render_pipeline_descriptor_from_obj(zend_object *o) { return METAL_OBJ(metal_mesh_render_pipeline_descriptor_t, o); }
static inline metal_stencil_descriptor_t *metal_stencil_descriptor_from_obj(zend_object *o) { return METAL_OBJ(metal_stencil_descriptor_t, o); }
static inline metal_compute_pipeline_descriptor_t *metal_compute_pipeline_descriptor_from_obj(zend_object *o) { return METAL_OBJ(metal_compute_pipeline_descriptor_t, o); }

/* ====================================================================
 *  Helper: read MTLSize from a PHP array [w, h, d]
 * ==================================================================== */

static inline MTLSize metal_parse_size(zval *arr) {
    MTLSize size = {1, 1, 1};
    HashTable *ht = Z_ARRVAL_P(arr);
    uint32_t count = zend_hash_num_elements(ht);
    zval *val;
    if ((val = zend_hash_index_find(ht, 0)) != NULL) size.width  = (NSUInteger)zval_get_long(val);
    if (count > 1 && (val = zend_hash_index_find(ht, 1)) != NULL) size.height = (NSUInteger)zval_get_long(val);
    if (count > 2 && (val = zend_hash_index_find(ht, 2)) != NULL) size.depth  = (NSUInteger)zval_get_long(val);
    return size;
}

/* ====================================================================
 *  Registration functions from metal_advanced.c
 * ==================================================================== */

void metal_register_event_classes(void);
void metal_register_heap_classes(void);
void metal_register_indirect_classes(void);
void metal_register_argument_encoder_class(void);
void metal_register_acceleration_classes(void);
void metal_register_binary_archive_classes(void);
void metal_register_compute_pass_descriptor_class(void);
void metal_register_advanced_constants(int module_number);
void metal_register_accel_encoder_class(void);
void metal_register_indirect_command_classes(void);
void metal_register_mesh_shader_classes(void);
void metal_register_stencil_descriptor_class(void);
void metal_register_compute_pipeline_descriptor_class(void);
void metal_register_final_constants(int module_number);

#endif /* METAL_INTERNAL_H */
