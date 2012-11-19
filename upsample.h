#ifndef TJF_UPSAMPLE_H
#define TJF_UPSAMPLE_H

#include <stdint.h>
#include "filter.h"
#ifdef __cplusplus
extern "C" {
#endif

/** Obvious/braindead upsampling filter. */
typedef struct _tjf_upsample {
  filter* parent;
  /** axis we'll upsample against */
  void (*set_axis)(void*, uint32_t axis);
  /** constant value/index to upsample out. */
  void (*set_index)(void*, uint64_t index);
  /** comparison function */
  void (*set_comparator)(void*, int(*compar)(const void*, const void*));
} upsample;

upsample* new_upsample();
void free_upsample(upsample*);

#ifdef __cplusplus
}
#endif
#endif
