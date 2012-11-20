#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "obj-write.h"
#ifndef M_PI
# define M_PI		3.14159265358979323846
#endif

static bool generic_obj(const char* obasename, const void* buf,
                        uint32_t width, uint32_t height, float zscaling,
                        size_t bytes)
{
  char* obj_filename = calloc(strlen(obasename)+8, sizeof(char));
  strcpy(obj_filename, obasename);
  strcat(obj_filename, ".obj");
  FILE* fp = fopen(obj_filename, "w+");
  free(obj_filename);
  if(!fp) {
    perror("couldn't open point cloud output file");
    return false;
  }
  fprintf(fp, "mtllib frame.mtl\n");

  uint64_t nverts = 0;
  /* foreach node, print out the vertex. */
  for(uint32_t y=0; y < height; ++y) {
    for(uint32_t x=0; x < width; ++x) {
      size_t idx = y*width + x;
      float val = 0.0f;
      switch(bytes) {
        case 2: val = (float) (((const uint16_t*)buf)[idx]); break;
        case 1: val = (float) (((const uint8_t*)buf)[idx]); break;
        default: abort();
      }
#if 0
      fprintf(fp, "v %f %f %f\n", (float)x, (float)y, val);
#else
      /* The entire mesh appears to be flipped around, at least in
       * meshlab's default coordinate system.  Rotate it 180 degrees around the
       * X axis. */
      const float fx = (float) x;
      const float fy = (float) y;
      const float fz = ((float) val) * zscaling;
#define TO_RAD(deg) (deg*M_PI/180.0)
      fprintf(fp, "v %f %f %f\n", fx,
              0 + cos(TO_RAD(180)) * fy - sin(TO_RAD(180)) * fz,
              0 + sin(TO_RAD(180)) * fy + cos(TO_RAD(180)) * fz);
#endif

      ++nverts;
    }
  }
  /* foreach node, print out the texture coordinates.  Note we create the same
   * number of texture coordinates as vertex coordinates. */
  for(uint32_t y=0; y < height; ++y) {
    for(uint32_t x=0; x < width; ++x) {
      /* what percent of the way through x and y are we through the image? */
      double xpercent = ((double)x) / (double)(width-1);
      /* 'height-y': reverse the image in Y */
      double ypercent = ((double)(height-y)) / (double)(height-1);
      fprintf(fp, "vt %lf %lf\n", xpercent, ypercent);
    }
  }

  fprintf(fp, "usemtl default\n");

  /* now foreach cell, so we can print the faces */
  for(uint32_t y=1; y < height-1; ++y) {
    for(uint32_t x=1; x < width-1; ++x) {
      /* (x,y) gives us the LOWER RIGHT of the quad we are forming.  Also the
       * +1 at the end comes because OBJ indexes from 1, not 0. */
      uint64_t topleft = ((y-1)*width + (x-1)) + 1;
      uint64_t topright = ((y-1)*width + x) + 1;
      uint64_t bottomleft = (y*width + (x-1)) + 1;
      uint64_t bottomright = (y*width + x) + 1;
      assert(topleft <= nverts);
      assert(topright <= nverts);
      assert(bottomleft <= nverts);
      assert(bottomright <= nverts);
      /* print out the face as two triangles.  The format is `V-index/VT-index'
       * where `V' is a vertex and `VT' is a vertex texture.  Since we have a
       * texture coord for every vertex, these are coincidentally the same. */
      fprintf(fp, "f %"PRIu64"/%"PRIu64" %"PRIu64"/%"PRIu64" "
                  "%"PRIu64"/%"PRIu64"\n", bottomleft, bottomleft, bottomright,
                                           bottomright, topleft, topleft);
      fprintf(fp, "f %"PRIu64"/%"PRIu64" %"PRIu64"/%"PRIu64" "
                  "%"PRIu64"/%"PRIu64"\n", bottomright, bottomright,
              topright, topright, topleft, topleft);
    }
  }

  fclose(fp);

  char* mtl_filename = calloc(strlen(obasename)+8, sizeof(char));
  strcpy(mtl_filename, obasename);
  strcat(mtl_filename, ".mtl");
  fp = fopen(mtl_filename, "w+");
  free(mtl_filename);
  if(!fp) {
    perror("could not create material file:");
    return false;
  }
  fprintf(fp, "newmtl default\n");
  fprintf(fp,
    "Ka 1.0 1.0 1.0\n"
    "Kd 1.0 1.0 1.0\n"
    "Ks 0.1 0.1 0.1\n"
    "d 1.0\n"
    "illum 2\n"
    "map_Ka cdata-01155.png\n"
    "map_Kd cdata-01155.png\n"
    "map_Ks cdata-01155.png\n"
  );
  fclose(fp);
  return true;
}

bool write_obj(const char* filename, const uint16_t* buf,
               uint32_t width, uint32_t height, float zscale) {
  return generic_obj(filename, buf, width, height, zscale, 2);
}
bool write_obj8(const char* filename, const uint8_t* buf,
                uint32_t width, uint32_t height, float zscale) {
  return generic_obj(filename, buf, width, height, zscale, 1);
}
