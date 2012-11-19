#ifndef SCENE_WRITE_CLOUD_H
#define SCENE_WRITE_CLOUD_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

bool write_cloud(const char* filename, const uint16_t* buf,
                 uint32_t width, uint32_t height);
bool write_cloud8(const char* filename, const uint8_t* buf,
                  uint32_t width, uint32_t height);

#ifdef __cplusplus
}
#endif
#endif
