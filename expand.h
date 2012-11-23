#ifndef TJF_EXPAND_Z_H
#define TJF_EXPAND_Z_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** expands a buffer out to FP.
 * The returned memory is dynamically allocated and must be 'free'd. */
float* expand8(uint8_t* buf, uint64_t width, uint64_t height);

#ifdef __cplusplus
}
#endif

#endif
