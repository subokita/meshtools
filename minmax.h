#ifndef TJF_MINMAX_H
#define TJF_MINMAX_H

#include <stdint.h>
#include "filter.h"
#ifdef __cplusplus
extern "C" {
#endif

/** Determines the minmax of a field. */
typedef struct _tjf_minmax {
  filter* parent;
} minmax;

minmax* new_minmax();
void free_minmax(minmax*);

#ifdef __cplusplus
}
#endif
#endif
