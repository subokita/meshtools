#!/bin/bash
# Takes two paths: one containing a bunch of depth maps, and another
# containing a series of color images.  Converts all the depth maps to
# meshes which are textured by the corresponding color image.

function error {
  echo $@
  exit 1
}
function usage {
  echo "$0: directory directory"
  exit 1
}

test -n "$1" || usage $@
test -n "$2" || usage $@
test -d "$1" || error "'$1' is not a directory!"
test -d "$2" || error "'$2' is not a directory!"

for depthmap in $(ls -1 $1/*png); do
  number=$(basename ${depthmap%%.png})
  number=${number##D}
  # we pipe to /dev/null because it might not have a texture: drop it, if so.
  texture=$(ls -1 ${2}/*${number}*png 2>/dev/null)
  # skip if we can't find an associated texture.
  if test -z "${texture}" ; then
    continue
  fi
  localtex=$(basename ${texture})
  echo "depthmap: ${depthmap}, ${number}, assoc tex: ${texture}, base: " \
       "${localtex}"
  cp ${texture} ${localtex} || error "copying texture failed."
  ../../meshdepth ${depthmap} ${localtex} ${number} || error "meshdepth failed!"
done
