#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "lerp.h"

float lerp(float v, float imin,float imax,
           float omin,float omax) {
  assert(imin <= v && v <= imax);
  return omin + (v-imin) * ((omax-omin)/(imax-imin));
}

static void
generic_lerp(bool fp, size_t bytes, void* buf, uint64_t width, uint64_t height,
             float imin,float imax, float omin,float omax) {
  for(uint64_t y=0; y < height; ++y) {
    for(uint64_t x=0; x < width; ++x) {
      const size_t idx = y*width + x;
      switch(bytes) {
        case 4: {
          if(fp) {
            float* data = (float*) buf;
            data[idx] = lerp(data[idx], imin,imax, omin,omax);
          } else {
            uint32_t* data = (uint32_t*) buf;
            data[idx] = lerp(data[idx], imin,imax, omin,omax);
          }
        } break;
        case 2: {
          uint16_t* data = (uint16_t*)buf;
          data[idx] = lerp(data[idx], imin,imax, omin,omax);
        } break;
        case 1: {
          uint8_t* data = (uint8_t*)buf;
          data[idx] = lerp(data[idx], imin,imax, omin,omax);
        } break;
        default: abort(); /* would be wrong anyway. */
      }
    }
  }
}

void lerp8(uint8_t* buf, uint64_t w, uint64_t h, float imin,float imax,
           float omin,float omax) {
  generic_lerp(false, 1, buf, w, h, imin,imax, omin,omax);
}
void lerpf(float* buf, uint64_t w, uint64_t h, float imin,float imax,
           float omin,float omax) {
  generic_lerp(true, 4, buf, w, h, imin,imax, omin,omax);
}
