#include <assert.h>
#include <stdlib.h>

#include "filter.h"

typedef struct _real_filter {
  filter public;

  void** inputs;
  void** outputs;
  bstream_t* itype;
  bstream_t* otype;
  size_t n_inputs;
  size_t n_outputs;
} filter_t;

static void f_cleanup(void*);
static void f_set_inputs(void*, void**, bstream_t*, size_t);
static void f_set_input(void*, void*, bstream_t);
static stream f_input(void*, size_t);
static void f_set_output(void*, void*, bstream_t);
static stream f_output(void*, size_t);

void* new_filter() {
  filter_t* f = calloc(sizeof(filter_t), 1);
  f->public.cleanup = f_cleanup;
  f->public.execute = NULL; /* abstract function! */
  f->public.set_inputs = f_set_inputs;
  f->public.set_input = f_set_input;
  f->public.input = f_input;
  f->public.set_output = f_set_output;
  f->public.output = f_output;

  f->inputs = NULL;
  f->outputs = NULL;
  f->itype = NULL;
  f->otype = NULL;
  f->n_inputs = 0;
  f->n_outputs = 0;
  return f;
}

void free_filter(filter* f) {
  f->cleanup(f);
  free(f);
}

static void f_cleanup(void* f) {
  filter_t* filt = (filter_t*) f;
  free(filt->inputs);
  free(filt->itype);
  for(size_t i=0; i < filt->n_outputs; ++i) {
    free(filt->outputs[i]);
  }
  free(filt->outputs);
  free(filt->otype);
}

static void f_set_inputs(void* f, void** inputs, bstream_t* strm, size_t n) {
  filter_t* filt = (filter_t*) f;

  filt->inputs = malloc(sizeof(void*) * n);
  filt->itype = malloc(sizeof(bstream_t) * n);
  for(size_t i=0; i < n; ++i) {
    filt->inputs[i] = inputs[i];
    filt->itype[i] = strm[i];
  }
  filt->n_inputs = n;
}

static void f_set_input(void* f, void* input, bstream_t strm) {
  filter_t* filt = (filter_t*) f;

  filt->inputs = malloc(sizeof(void*) * 1);
  filt->itype = malloc(sizeof(bstream_t) * 1);
  filt->inputs[0] = input;
  filt->itype[0] = strm;
  filt->n_inputs = 1;
}

static stream f_input(void* f, size_t idx) {
  filter_t* filt = (filter_t*) f;
  stream strm;
  strm.data = NULL;

  assert(idx < filt->n_inputs);
  if(idx >= filt->n_inputs) { /* just bail / return invalid data. */
    return strm;
  }
  strm.data = filt->inputs[idx];
  strm.info = filt->itype[idx];
  return strm;
}

static void f_set_output(void* f, void* data, bstream_t strm)
{
  filter_t* filt = (filter_t*) f;

  filt->outputs = malloc(sizeof(void*) * 1);
  filt->otype = malloc(sizeof(bstream_t) * 1);
  filt->outputs[0] = data;
  filt->otype[0] = strm;
  filt->n_outputs = 1;
}

static stream f_output(void* self, size_t idx) {
  filter_t* filt = (filter_t*) self;
  stream strm;
  strm.data = NULL;

  assert(idx < filt->n_outputs);
  if(idx >= filt->n_outputs) { /* just bail / return invalid data. */
    return strm;
  }
  strm.data = filt->outputs[idx];
  strm.info = filt->otype[idx];
  return strm;
}
