#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cloud.h"
#include "wrpng.h"

int main(int argc, char* argv[]) {
  if(argc != 3) {
    fprintf(stderr, "Usage: %s depthimage obj-file\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  uint8_t* img;
  uint32_t width, height;
  readpng(argv[1], &img, &width, &height);
  write_cloud8("frame.obj", img, width, height);
  free(img);

  return EXIT_SUCCESS;
}