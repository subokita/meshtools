#ifndef TJF_CROP_H
#define TJF_CROP_H

#include <stdint.h>
#include "filter.h"
#ifdef __cplusplus
extern "C" {
#endif

/** Simple cropping filter.  Takes a dimension (or axis), a value or
    index , and a comparison function. returns the data set which
    satisfies the comparison function. */
typedef struct _crop {
  filter* parent;
  /** axis we'll crop against */
  void (*set_axis)(void*, uint32_t axis);
  /** constant value/index to crop out. */
  void (*set_index)(void*, uint64_t index);
  /** comparison function */
  void (*set_comparator)(void*, int(*compar)(const void*, const void*));
} crop;

crop* new_crop();
void free_crop(crop*);

#ifdef __cplusplus
}
#endif
#endif
