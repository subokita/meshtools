#include <stdlib.h>
#include "scale.h"

/** scales a given buffer by a constant value. */
static void
generic_scale(size_t bytes, void* buf, uint64_t width, uint64_t height,
              float scale) {
  for(uint64_t y=0; y < height; ++y) {
    for(uint64_t x=0; x < width; ++x) {
      const size_t idx = y*width + x;
      switch(bytes) {
        case 2: {
          uint16_t* data = (uint16_t*)buf;
          data[idx] = data[idx] * scale;
        } break;
        case 1: {
          uint8_t* data = (uint8_t*)buf;
          data[idx] = data[idx] * scale;
        } break;
        default: abort(); /* would be wrong anyway. */
      }
    }
  }
}

void scale8(uint8_t* buf, uint64_t w, uint64_t h, float scale) {
  generic_scale(1, buf, w, h, scale);
}
