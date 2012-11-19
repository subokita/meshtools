#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "filter.h"
#include "crop.h"

typedef struct _crop_real {
  crop public;
  uint32_t axis;
  uint64_t index;
  int (*comparator)(const void*, const void*);
} crop_t;

static bool cr_execute(void* self);
static void cr_set_axis(void*, uint32_t);
static void cr_set_index(void*, uint64_t);
static void cr_set_comparator(void*, int(*compar)(const void*, const void*));

crop* new_crop() {
  crop_t* crp = calloc(sizeof(crop_t), 1);
  crp->public.parent = new_filter();
  crp->public.parent->execute = cr_execute;
  crp->public.set_axis = cr_set_axis;
  crp->public.set_index = cr_set_index;
  crp->public.set_comparator = cr_set_comparator;
  return (crop*)crp;
}

void free_crop(crop* c) {
  crop_t* crop = (crop_t*) c;
  /* local `cleanup' */
  crop->axis = 0;
  crop->index = 0ULL;
  crop->comparator = NULL;

  free_filter(crop->public.parent);
  free(c);
}

static bool cr_execute(void* self) {
  crop_t* crop = (crop_t*) self;

  assert(crop->comparator);
  if(crop->comparator == NULL) {
    fprintf(stderr, "comparator not set.\n");
    return false;
  }

  /* 0, statically: we only accept one input */
  stream strm = crop->public.parent->input(crop->public.parent, 0);
  assert(valid_stream(strm.info));
  if(!valid_stream(strm.info)) {
    fprintf(stderr, "stream is nonsense, coding error.\n");
    return false;
  }
  bstream_t info = strm.info;

  /* just an alias to make typing shorter */
  const size_t idims[4] = { info.dimensions[0], info.dimensions[1],
                            info.dimensions[2], info.dimensions[3] };
  /* output dimensions are the same as our input dims... */
  size_t dims[4] = { idims[0], idims[1], idims[2], idims[3] };
  /* ... except one of our axes is now shorter. */
  dims[crop->axis] = crop->index;
  stream outs;

  if(info.type == UNSIGNED_INTEGER && info.width == 1) {
    fprintf(stderr, "uint8 data not yet supported.\n");
    return false;
  } else if(info.type == UNSIGNED_INTEGER && info.width == 2) {
    if(info.channels != 1) {
      fprintf(stderr, "bug: crop doesn't handle multi-channel data.\n");
      return false;
    }

    const uint16_t* din = (const uint16_t*)strm.data;
    uint16_t* dout = calloc(dims[0] * dims[1] * dims[2] * dims[3],
                            sizeof(uint16_t));
    for(size_t t=0; t < dims[3]; ++t) {
      for(size_t z=0; z < dims[2]; ++z) {
        for(size_t y=0; y < dims[1]; ++y) {
          for(size_t x=0; x < dims[0]; ++x) {
            size_t oidx = t * dims[2] * dims[1] * dims[0] +
                          z * dims[1] * dims[0] +
                          y * dims[0] + x;
            size_t iidx = t * idims[2] * idims[1] * idims[0] +
                          z * idims[1] * idims[0] +
                          y * idims[0] + x;
            dout[oidx] = din[iidx];
          }
        }
      }
    }
    outs.data = dout;
    outs.info.type = UNSIGNED_INTEGER;
    outs.info.width = sizeof(uint16_t);
    outs.info.channels = 1;
    outs.info.dimensions[0] = dims[0];
    outs.info.dimensions[1] = dims[1];
    outs.info.dimensions[2] = dims[2];
    outs.info.dimensions[3] = dims[3];
  } else {
    fprintf(stderr, "??? data not supported.\n");
    return false;
  }

  crop->public.parent->set_output(crop->public.parent, outs.data, outs.info);
  return true;
}
static void cr_set_axis(void* self, uint32_t axis) {
  ((crop_t*)self)->axis = axis;
}
static void cr_set_index(void* self, uint64_t index) {
  ((crop_t*)self)->index = index;
}
static void cr_set_comparator(void* self,
                              int(*compar)(const void*, const void*)) {
  ((crop_t*)self)->comparator = compar;
}
