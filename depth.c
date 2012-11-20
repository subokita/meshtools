#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "obj-write.h"
#include "wrpng.h"

int main(int argc, char* argv[]) {
  if(argc != 3) {
    fprintf(stderr, "Usage: %s depthimage obj-file\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  uint8_t* img;
  uint32_t width, height;
  readpng(argv[1], &img, &width, &height);
  write_obj8("frame", "cdata-01155.png", img, width, height, 0.1f);
  free(img);

  return EXIT_SUCCESS;
}
