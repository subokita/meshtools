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
  if(argc != 4) {
    fprintf(stderr, "Usage: %s depthimage colorimage obj-file\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  const char* depthfile = argv[1];
  const char* colorfile = argv[2];
  const char* objfile = argv[3];

  uint8_t* img;
  uint32_t width, height;

  readpng(depthfile, &img, &width, &height);

  {
    /* generate the basename for the OBJ file by trying to find a "."
    and hacking off everything there and later. */
    char* base_obj = calloc(strlen(objfile)+1, sizeof(char));
    strcpy(base_obj, objfile);
    char* dot = strrchr(base_obj, '.');
    if(dot) { *dot = '\0'; }

    write_obj8(base_obj, colorfile, img, width, height, 0.1f);
    free(base_obj);
  }

  free(img);

  return EXIT_SUCCESS;
}
