/*
  +----------------------------------------------------------------------+
  | php-metal - PHP Extension for Apple Metal GPU API                    |
  +----------------------------------------------------------------------+
  | Copyright (c) 2026                                                   |
  +----------------------------------------------------------------------+
  | Licensed under the MIT License                                       |
  +----------------------------------------------------------------------+
*/

#ifndef PHP_METAL_H
#define PHP_METAL_H

extern zend_module_entry metal_module_entry;
#define phpext_metal_ptr &metal_module_entry

#define PHP_METAL_VERSION "0.1.0"

#ifdef ZTS
#include "TSRM.h"
#endif

/* ------------------------------------------------------------------ */
/*  Forward declarations for class entries                             */
/* ------------------------------------------------------------------ */

extern zend_class_entry *metal_ce_device;
extern zend_class_entry *metal_ce_buffer;
extern zend_class_entry *metal_ce_command_queue;
extern zend_class_entry *metal_ce_command_buffer;
extern zend_class_entry *metal_ce_compute_encoder;
extern zend_class_entry *metal_ce_library;
extern zend_class_entry *metal_ce_metal_function;
extern zend_class_entry *metal_ce_compute_pipeline_state;
extern zend_class_entry *metal_ce_render_pipeline_state;
extern zend_class_entry *metal_ce_render_encoder;
extern zend_class_entry *metal_ce_texture;
extern zend_class_entry *metal_ce_texture_descriptor;
extern zend_class_entry *metal_ce_render_pipeline_descriptor;
extern zend_class_entry *metal_ce_render_pass_descriptor;
extern zend_class_entry *metal_ce_color_attachment_descriptor;
extern zend_class_entry *metal_ce_depth_stencil_state;
extern zend_class_entry *metal_ce_depth_stencil_descriptor;
extern zend_class_entry *metal_ce_vertex_descriptor;
extern zend_class_entry *metal_ce_vertex_buffer_layout_descriptor;
extern zend_class_entry *metal_ce_vertex_attribute_descriptor;
extern zend_class_entry *metal_ce_sampler_state;
extern zend_class_entry *metal_ce_sampler_descriptor;
extern zend_class_entry *metal_ce_blit_encoder;
extern zend_class_entry *metal_ce_drawable;
extern zend_class_entry *metal_ce_exception;

/* ------------------------------------------------------------------ */
/*  PHP version compatibility                                          */
/* ------------------------------------------------------------------ */

/* ZEND_ACC_NO_DYNAMIC_PROPERTIES was added in 8.2 */
#ifndef ZEND_ACC_NO_DYNAMIC_PROPERTIES
#define ZEND_ACC_NO_DYNAMIC_PROPERTIES 0
#endif

/* Compatibility for older PHP versions */
#if PHP_VERSION_ID < 80300
#define ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(pass_by_ref, name, type_hint, allow_null, default_value) \
    ZEND_ARG_TYPE_INFO(pass_by_ref, name, type_hint, allow_null)
#endif

#endif /* PHP_METAL_H */
