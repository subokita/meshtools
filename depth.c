#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>
#include "expand.h"
#include "lerp.h"
#include "obj-write.h"
#include "tjf-write.h"
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

  printf("expanding...\n");
  float* imgf = expand8(img, width, height);
  free(img);

  printf("lerping...\n");
  lerpf(imgf, width,height, 0.0f,241.0f, 1.0f,1.5f);

#if 0
  {
    printf("writing OBJ...\n");
    /* generate the basename for the OBJ file by trying to find a "."
    and hacking off everything there and later. */
    char* base_obj = calloc(strlen(objfile)+1, sizeof(char));
    strcpy(base_obj, objfile);
    char* dot = strrchr(base_obj, '.');
    if(dot) { *dot = '\0'; }

    write_objf(base_obj, colorfile, imgf, width, height);
    free(base_obj);
  }
#endif
  {
    printf("writing TJF...\n");
    /* generate the basename for the filename by trying to find a "."
    and hacking off everything there and later. */
    char* base_tjf = calloc(strlen(objfile)+1, sizeof(char));
    strcpy(base_tjf, objfile);
    char* dot = strrchr(base_tjf, '.');
    if(dot) { *dot = '\0'; }

    write_tjff(16, base_tjf, colorfile, imgf, width, height);
    free(base_tjf);
  }

  return EXIT_SUCCESS;
}
