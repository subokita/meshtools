#ifndef SCENE_WRITE_CLOUD_H
#define SCENE_WRITE_CLOUD_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

bool write_obj(const char* filename, const char* texture, const uint16_t* buf,
               uint32_t width, uint32_t height);
bool write_obj8(const char* filename, const char* texture, const uint8_t* buf,
                uint32_t width, uint32_t height);
bool write_objf(const char* filename, const char* texture, const float* buf,
                uint32_t width, uint32_t height);

#ifdef __cplusplus
}
#endif
#endif
