#include "mruby/lz4.h"
#include "lz4.h"
#include <mruby/string.h>

#if (__GNUC__ >= 3) || (__INTEL_COMPILER >= 800) || defined(__clang__)
# define likely(x) __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#else
# define likely(x) (x)
# define unlikely(x) (x)
#endif

static mrb_value
mrb_LZ4_compress_default(mrb_state *mrb, mrb_value self)
{
  char *source;
  mrb_int source_size;

  mrb_get_args(mrb, "s", &source, &source_size);

  int maxDestSize = LZ4_COMPRESSBOUND(source_size);
  if (likely(maxDestSize)) {
    mrb_value dest = mrb_str_buf_new(mrb, maxDestSize);
    int destSize = LZ4_compress_default(source, RSTRING_PTR(dest), source_size, RSTRING_CAPA(dest));
    if (likely(destSize)) {
      return mrb_str_resize(mrb, dest, destSize);
    } else {
      mrb_raise(mrb, E_LZ4_ERROR, "cannot compress");
    }
  } else {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "source_size is too large");
  }
}

static mrb_value
mrb_LZ4_decompress_safe(mrb_state *mrb, mrb_value self)
{
  char *source;
  mrb_int compressedSize, maxDecompressedSize;

  mrb_get_args(mrb, "si", &source, &compressedSize, &maxDecompressedSize);

  if (unlikely(maxDecompressedSize < 0)) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "maxDecompressedSize mustn't be negative");
  }

  mrb_value dest = mrb_str_buf_new(mrb, maxDecompressedSize);

  int decompressedSize = LZ4_decompress_safe(source, RSTRING_PTR(dest), compressedSize, RSTRING_CAPA(dest));
  if (decompressedSize >= 0) {
    return mrb_str_resize(mrb, dest, decompressedSize);
  } else {
    mrb_raise(mrb, E_LZ4_ERROR, "cannot decompress, source may be malformed or maxDecompressedSize is too small");
  }
}


void
mrb_mruby_lz4_gem_init(mrb_state* mrb) {
  struct RClass *LZ4_mod;

  LZ4_mod = mrb_define_module(mrb, "LZ4");
  mrb_define_class_under(mrb, LZ4_mod, "Error", E_RUNTIME_ERROR);
  mrb_define_module_function(mrb, LZ4_mod, "compress_default", mrb_LZ4_compress_default, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, LZ4_mod, "decompress_safe", mrb_LZ4_decompress_safe, MRB_ARGS_REQ(2));
}

void
mrb_mruby_lz4_gem_final(mrb_state* mrb) {
}
