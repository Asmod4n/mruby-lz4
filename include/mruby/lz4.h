#ifndef MRUBY_LZ4_H
#define MRUBY_LZ4_H

#include <mruby.h>

#ifdef __cplusplus
extern "C" {
#endif

#define E_LZ4_ERROR (mrb_class_get_under(mrb, mrb_module_get(mrb, "LZ4"), "Error"))

#ifdef __cplusplus
}
#endif

#endif
