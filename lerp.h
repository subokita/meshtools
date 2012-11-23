#ifndef TJF_LERP_Z_H
#define TJF_LERP_Z_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** scales the given buffer from one range to another, linearly.
 * @argument imin minimum value in input range
 * @argument imax maximum value in output range */
void lerp8(uint8_t* buf, uint64_t width, uint64_t height,
           float imin,float imax,
           float omin,float omax);

void lerpf(float* buf, uint64_t width, uint64_t height,
           float imin,float imax,
           float omin,float omax);

float lerp(float v, float imin,float imax,
           float omin,float omax);

#ifdef __cplusplus
}
#endif

#endif
