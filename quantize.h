#ifndef TJF_QUANTIZE_H
#define TJF_QUANTIZE_H

#include <stdint.h>
#include "filter.h"
#ifdef __cplusplus
extern "C" {
#endif

/** Quantizes a data set.
 * @argument data set to quantize
 * @argument 2-element array with min/max of the above data set */
typedef struct _tjf_quantize {
  filter* parent;
} quantize;

quantize* new_quantize();
void free_quantize(quantize*);

#ifdef __cplusplus
}
#endif
#endif
