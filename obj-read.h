#ifndef TJF_OBJ_READ
#define TJF_OBJ_READ
#include <stdio.h>
#include "geometry.h"
#ifdef __cplusplus
extern "C" {
#endif

/** Reads the set of vertices from the given OBJ file.  Assumes the stream is
 * positioned such that a read will give a vertex.  Allocates vertex array.
 * @returns number of elements in vertex array. */
uint64_t obj_read_vertices(FILE* fp, vertex**);
uint64_t obj_read_texcoords(FILE* fp, texcoord**);
uint64_t obj_read_faces(FILE* fp, face**);

#ifdef __cplusplus
}
#endif
#endif
