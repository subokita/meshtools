
#include <stdlib.h>

#include "expand.h"

float*
generic_expand(size_t bytes, void* buf, uint64_t width, uint64_t height) {
  float* fv = malloc(sizeof(float)*width*height);

  for(uint64_t y=0; y < height; ++y) {
    for(uint64_t x=0; x < width; ++x) {
      const size_t idx = y*width + x;
      switch(bytes) {
        case 2: fv[idx] = (float)(((const uint16_t*)buf)[idx]); break;
        case 1: fv[idx] = (float)(((const  uint8_t*)buf)[idx]); break;
        default: abort(); /* would do wrong thing anyway */
      }
    }
  }
  return fv;
}

float* expand8(uint8_t* buf, uint64_t width, uint64_t height) {
  return generic_expand(1, buf, width, height);
}
