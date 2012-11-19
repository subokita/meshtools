#ifndef TJF_FILTER_H
#define TJF_FILTER_H
#include <stdbool.h>
#include "bstream.h"
#ifdef __cplusplus
extern "C" {
#endif

/** a stream plus a description of it.
 * should probably be named a 'tstream' ('tom-stream') or something... */
typedef struct _fake_stream {
  void* data;
  bstream_t info;
} stream;

typedef struct _fake_filter {
  /** executes the filter.  abstract function. */
  bool (*execute)(void* self);
  /** sets the inputs to the filter */
  void (*set_inputs)(void* self, void**, bstream_t*, size_t n_inputs);
  /** convenience for the common case of one input */
  void (*set_input)(void* self, void*, bstream_t);
  /** returns the `i'th input. */
  stream (*input)(void* self, size_t i);

  /** sets an output variable, and says that we have one output. */
  void (*set_output)(void* self, void*, bstream_t);
  /** grabs the `i'th output. */
  stream (*output)(void* self, size_t i);

  /** internal; destructor. */
  void (*cleanup)(void* self);
} filter;

void* new_filter();
void free_filter(filter*);

#ifdef __cplusplus
}
#endif
#endif
