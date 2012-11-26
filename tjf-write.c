#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "tjf-write.h"
#include "lerp.h"
#ifndef M_PI
# define M_PI		3.14159265358979323846
#endif

enum DataType {
  DT_UINT8=0, DT_UINT16=1, DT_UINT32=2, DT_UINT64=3,
  DT_INT8=4, DT_INT16=5, DT_INT32=6, DT_INT64=7,
  DT_FLOAT=8, DT_DOUBLE=9
};

/* broken: needs to take sign information, but we ignore that for now */
static enum DataType
data_type(bool flp, size_t bytes) {
  switch(bytes) {
    case 8: if(flp) { return DT_DOUBLE; } else { return DT_UINT64; }; break;
    case 4: if(flp) { return DT_FLOAT; } else { return DT_UINT32; }; break;
    case 2: return DT_UINT16;
    case 1: return DT_UINT8;
    default: abort();
  }
}

bool
generic_tjfwrite(bool flp, size_t bytes, const char* tbasename,
                 const char* texture, const float* buf,
                 uint32_t width, uint32_t height) {
  char* tjf_filename = calloc(strlen(tbasename)+8, sizeof(char));
  strcpy(tjf_filename, tbasename);
  strcat(tjf_filename, ".tjf");
  FILE* fp = fopen(tjf_filename, "w+b");
  free(tjf_filename);
  if(!fp) {
    perror("couldn't open tjf mesh file");
    return false;
  }

  { /* write simple header data.  Pretty simple: */
    /* the literal 4 bytes: "TJF1".  this is the file magic. */
    /* uint16_t: # chars in filename for texture */
    /* the filename of the image to use as a texture */
    /* uint64_t: the number of vertices */
    /* uint64_t: the number of texture coords */
    /* uint64_t: the number of triangles */
    /* uint64_t: enum detailing the data type */
    const char magic[4] = {"TJF1"};
    fwrite(magic, sizeof(char), 4, fp);
    uint16_t len = (uint16_t) strlen(texture);
    fwrite(&len, sizeof(uint16_t), 1, fp);
    fwrite(texture, sizeof(char), len, fp);
    uint64_t nverts = width * height;
    fwrite(&nverts, sizeof(uint64_t), 1, fp);
    /* texcoords == vert coords */
    fwrite(&nverts, sizeof(uint64_t), 1, fp);
    uint64_t triangles = (width-2)*(height-2)*2;
    fwrite(&triangles, sizeof(uint64_t), 1, fp);
    enum DataType dtype = data_type(flp, bytes);
    uint64_t u64_dtype = (uint64_t) dtype;
    fwrite(&u64_dtype, sizeof(uint64_t), 1, fp);
  }
  /* output the vertex data. */
  uint64_t nverts = 0;
  for(uint64_t y=0; y < height; ++y) {
    for(uint64_t x=0; x < width; ++x) {
      const size_t idx = y*width + x;
      float val = 0.0f;
      switch(bytes) {
        case 4: {
          if(flp) {
            val = (float) (((const float*)buf)[idx]);
          } else {
            val = (float) (((const uint32_t*)buf)[idx]);
          }
        } break;
        case 2: val = (float) (((const uint16_t*)buf)[idx]); break;
        case 1: val = (float) (((const uint8_t*)buf)[idx]); break;
        default: abort();
      }

      const float w = (float) width;
      const float h = (float) height;
      float towrite[3];
      towrite[0] = (float) x - (w/2.0f);
      towrite[1] = (float) y - (h/2.0f);
      towrite[2] = (float) val;

      /* normalize X/Y coords to fit in the unit cube */
      towrite[0] = lerp(towrite[0], (0-(w/2)),w/2, -2.0f,2.0f);
      towrite[1] = lerp(towrite[1], (0-(h/2)),h/2, -2.0f,2.0f);
      assert(-2.0f <= towrite[0] && towrite[0] <= 2.0f);
      assert(-2.0f <= towrite[1] && towrite[1] <= 2.0f);

      /* The entire mesh appears to be flipped around.
       * Rotate it 180 degrees around the X axis. */
#define TO_RAD(deg) (deg*M_PI/180.0)
      towrite[0] = towrite[0];
      towrite[1] = 0 + cos(TO_RAD(180)) * towrite[1]
                     - sin(TO_RAD(180)) * towrite[2];
      towrite[2] = 0 + sin(TO_RAD(180)) * towrite[1]
                     + cos(TO_RAD(180)) * towrite[2];
      fwrite(towrite, sizeof(float), 3, fp);
      ++nverts;
    }
  }
  assert(nverts == width*height);

  /* foreach node, print out the texture coordinates.  Note we create the same
   * number of texture coordinates as vertex coordinates. */
  for(uint32_t y=0; y < height; ++y) {
    for(uint32_t x=0; x < width; ++x) {
      /* what percent of the way through x and y are we through the image? */
      float percent[2] = {
        ((double)x) / (double)(width-1),
        ((double)y) / (double)(height-1)
      };
      fwrite(percent, sizeof(float), 2, fp);
    }
  }

  /* now we can print the triangle information */
  uint64_t n_tris=0;
  for(uint32_t y=1; y < height-1; ++y) {
    for(uint32_t x=1; x < width-1; ++x) {
      /* (x,y) gives us the LOWER RIGHT of the quad we are forming. */
      uint64_t topleft = ((y-1)*width + (x-1));
      uint64_t topright = ((y-1)*width + x);
      uint64_t bottomleft = (y*width + (x-1));
      uint64_t bottomright = (y*width + x);
      assert(topleft <= nverts);
      assert(topright <= nverts);
      assert(bottomleft <= nverts);
      assert(bottomright <= nverts);
      uint32_t tri1[3] = { bottomleft, bottomright, topleft };
      uint32_t tri2[3] = { bottomright, topright, topleft };
      fwrite(tri1, sizeof(uint32_t), 3, fp);
      fwrite(tri2, sizeof(uint32_t), 3, fp);
      n_tris += 2; /* two triangles written! */
    }
  }
  if(n_tris != ((width-2)*(height-2)*2)) {
    /* %lu is wrong, should be PRI_u64 or whatever.. */
    g_error("%lu triangles, only %lu verts!", n_tris, nverts);
  }
  
  fclose(fp);
  return true;
}

bool write_tjff(const char* filename, const char* texture, const float* buf,
                uint32_t width, uint32_t height)
{
  return generic_tjfwrite(true, 4, filename, texture, buf, width, height);
}
