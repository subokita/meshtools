#ifndef TJF_TJF_WRITE_H
#define TJF_TJF_WRITE_H
/* Writes a custom 'tjf' mesh format file type. */

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

bool write_tjff(int32_t downsample, const char* filename, const char* texture,
                const float* buf, uint32_t width, uint32_t height);

#ifdef __cplusplus
}
#endif
#endif
