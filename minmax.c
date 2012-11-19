#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "bstream.h"
#include "minmax.h"

typedef struct _minmax_real {
  minmax public;
} minmax_t;

static bool mm_execute(void* self);

minmax* new_minmax() {
  minmax_t* mm = calloc(sizeof(minmax_t), 1);
  mm->public.parent = new_filter();
  mm->public.parent->execute = mm_execute;
  return (minmax*)mm;
}

void free_minmax(minmax* m) {
  minmax_t* mm = (minmax_t*) m;
  free_filter(mm->public.parent);
  free(m);
}

/* okay this is a bit nuanced, be careful.
 * We want a generic min function.  Therefore it must accept and return void*s.
 * But how do we use the function?  we want to say:
 *    for(i=0 to whatever):
 *      min = minfunction(min, buffer[i]);
 * But the trouble is we can't index 'buffer' without knowing it's type.  so
 * the function takes the buffer and casts it properly itself. */
#define MINFUNC(name, type) \
static const void* name(const void* a, const void* buffer, size_t idx) { \
  const type* l = (const type*) a; \
  const type* r = ((const type*) buffer) + idx; \
  return *l < *r ? a : (const void*)(((const uint8_t*) buffer) + idx); \
}
#define MAXFUNC(name, type) \
static const void* name(const void* a, const void* buffer, size_t idx) { \
  const type* l = (const type*) a; \
  const type* r = ((const type*) buffer) + idx; \
  return *l > *r ? a : (const void*)(((const uint8_t*) buffer) + idx); \
}
MINFUNC(minu8, uint8_t)
MINFUNC(minu16, uint16_t)
MAXFUNC(maxu8, uint8_t)
MAXFUNC(maxu16, uint16_t)

static bool mm_execute(void* self) {
  minmax_t* mm = (minmax_t*) self;

  /* 0, statically: we only accept one input, for no real good reason.. */
  stream strm = mm->public.parent->input(mm->public.parent, 0);
  assert(valid_stream(strm.info));
  if(!valid_stream(strm.info)) {
    fprintf(stderr, "stream is nonsense, coding error.\n");
    return false;
  }

  if(strm.info.channels != 1) {
    fprintf(stderr, "only single-channel data is currently supported.\n");
    return false;
  }

  bstream_t info = strm.info;
  const void* (*minf)(const void* a, const void* buffer, size_t i) = NULL;
  const void* (*maxf)(const void* a, const void* buffer, size_t i) = NULL;
  if(info.type == UNSIGNED_INTEGER && info.width == 1) {
    minf = minu8;
    maxf = maxu8;
  } else if(info.type == UNSIGNED_INTEGER && info.width == 2) {
    minf = minu16;
    maxf = maxu16;
  } else {
    fprintf(stderr, "unhandled type\n");
    return false;
  }

  /* alias for shorter typing + do the casting for us */
  const size_t dims[4] = { info.dimensions[0], info.dimensions[1],
                           info.dimensions[2], info.dimensions[3] };
  const void* min = strm.data;
  const void* max = strm.data;
  const size_t indices = dims[0] * dims[1] * dims[2] * dims[3];
  for(size_t i=0; i < indices; ++i) {
    min = minf(min, strm.data, i);
    max = maxf(max, strm.data, i);
  }

  void* data;
  if(info.type == UNSIGNED_INTEGER && info.width == 1) {
    data = malloc(sizeof(uint8_t) * 2);
    ((uint8_t*)data)[0] = *(const uint8_t*) min;
    ((uint8_t*)data)[1] = *(const uint8_t*) max;
  } else if(info.type == UNSIGNED_INTEGER && info.width == 2) {
    data = malloc(sizeof(uint16_t) * 2);
    ((uint16_t*)data)[0] = *(const uint16_t*) min;
    ((uint16_t*)data)[1] = *(const uint16_t*) max;
  }
  bstream_t outinfo;
  outinfo.type = info.type;
  outinfo.width = info.width;
  outinfo.channels = 1; /* hack */
  outinfo.dimensions[0] = 2;
  outinfo.dimensions[1] = 1;
  outinfo.dimensions[2] = 1;
  outinfo.dimensions[3] = 1;
  
  mm->public.parent->set_output(mm->public.parent, data, outinfo);
  return true;
}
