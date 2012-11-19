#include <stdlib.h>
#include "quantize.h"

typedef struct _quantize_real {
  quantize public;
} quantize_t;

static bool quant_execute(void* self);

quantize* new_quantize() {
  quantize_t* quant = calloc(sizeof(quantize_t), 1);
  quant->public.parent = new_filter();
  quant->public.parent->execute = quant_execute;
  return (quantize*)quant;
}
void free_quantize(quantize* q) {
  quantize_t* quant = (quantize_t*) q;
  free_filter(quant->public.parent);
  free(q);
}

static bool quant_execute(void* self) {
  quantize_t* quant = (quantize_t*) self;

  stream datastrm = quant->public.parent->input(quant->public.parent, 0);
  stream mmstrm = quant->public.parent->input(quant->public.parent, 1);
  assert(valid_stream(datastrm));
  assert(valid_stream(mmstrm));
  if(!valid_stream(datastrm) || !valid_stream(mmstrm)) {
    fprintf(stderr, "stream[s] are nonsense, coding error.\n");
    return false;
  }
  const size_t indices = (size_t) (
    datastrm.info.dimensions[0] * datastrm.info.dimensions[1] *
    datastrm.info.dimensions[2] * datastrm.info.dimensions[3]
  );

  if(info.type == UNSIGNED_INTEGER && info.width == 2) {
    const uint16_t* din = (const uint16_t*) datastrm.data;
    uint8_t* dout = calloc(indices, sizeof(uint8_t));
    /* FIXME here. */
    for(size_t i=0; i < indices; ++i) {
      datum[i] = lerp(datum[i], minmax[0],minmax[1], 0,256);
    }
  } else {
    fprintf(stderr, "unhandled type..\n");
    return false;
  }
}
