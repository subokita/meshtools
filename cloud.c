#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "cloud.h"

#if 0
static bool generic_ply(const char* filename, const void* buf,
                        uint32_t width, uint32_height, size_t bytes)
{
}
#endif

static bool generic_cloud(const char* filename, const void* buf,
                          uint32_t width, uint32_t height, size_t bytes)
{
  FILE* fp = fopen(filename, "w+");
  if(!fp) {
    perror("couldn't open point cloud output file");
    return false;
  }
  /* fprintf(fp, "mtllib frame.mtl\n"); */

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
      fprintf(fp, "v %f %f %f\n", (float)x, (float)y, val);
      ++nverts;
    }
  }
  /* foreach node, print out the texture coordinates.  Note we create the same
   * number of texture coordinates as vertex coordinates. */
  for(uint32_t y=0; y < height; ++y) {
    for(uint32_t x=0; x < width; ++x) {
      /* what percent of the way through x and y is that through the image? */
      double xpercent = ((double)x) / (double)(width-1);
      double ypercent = ((double)y) / (double)(height-1);
      fprintf(fp, "vt %lf %lf\n", xpercent, ypercent);
    }
  }

  /* fprintf(fp, "usemtl default\n"); */
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
#if 0
      fprintf(fp, "f %"PRIu64" %"PRIu64" %"PRIu64"\n", bottomright, topright,
              topleft);
#endif
    }
  }

  fclose(fp);
#if 0
  fp = fopen("frame.mtl", "w+");
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
    "map_Ka cdata.png\n"
    "map_Kd cdata.png\n"
    "map_Ks cdata.png\n"
  );
  fclose(fp);
#endif
  return true;
}

bool write_cloud(const char* filename, const uint16_t* buf,
                 uint32_t width, uint32_t height) {
  return generic_cloud(filename, buf, width, height, 2);
}
bool write_cloud8(const char* filename, const uint8_t* buf,
                  uint32_t width, uint32_t height) {
  return generic_cloud(filename, buf, width, height, 1);
}
