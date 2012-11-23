#ifndef TJF_SCALE_Z_H
#define TJF_SCALE_Z_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** scales a given buffer by a constant value. */
void scale8(uint8_t* buf, uint64_t width, uint64_t height, float scale);

#ifdef __cplusplus
}
#endif

#endif
