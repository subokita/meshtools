/** identifies the range of all depth values. */
#include <assert.h>
#include <errno.h>
#include <float.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "obj-write.h"
#include "wrpng.h"

int main(int argc, char* argv[]) {
  if(argc != 2) {
    fprintf(stderr, "Usage: %s depthimage\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  const char* depthfile = argv[1];

  uint8_t* img;
  uint32_t width, height;

  if(!readpng(depthfile, &img, &width, &height)) {
    fprintf(stderr, "could not open '%s'!\n", depthfile);
    return EXIT_FAILURE;
  }
  double mm[2] = {FLT_MAX, -FLT_MAX};
  for(uint32_t j=0; j < height; ++j) {
    for(uint32_t i=0; i < width; ++i) {
      const size_t idx = j*width + i;
      const float val = (float)(img[idx]);
      if(val < mm[0]) { mm[0] = val; }
      if(val > mm[1]) { mm[1] = val; }
    }
  }
  printf("minmax depth: %lf %lf\n", mm[0], mm[1]);

  free(img);

  return EXIT_SUCCESS;
}
