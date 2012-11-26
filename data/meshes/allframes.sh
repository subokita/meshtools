#!/bin/bash
# Takes two paths: one containing a bunch of depth maps, and another
# containing a series of color images.  Converts all the depth maps to
# meshes which are textured by the corresponding color image.

function error {
  echo $@
  exit 1
}
function usage {
  echo "$0: depth-directory color-directory texblurred-directory"
  exit 1
}

SCENE="/home/tfogal/dev/scenelib/"
REN="/home/tfogal/dev/scenelib/renderer/renderer"
export LD_LIBRARY_PATH="${SCENE}"

test -n "$1" || usage $@
test -n "$2" || usage $@
test -n "$3" || usage $@
test -d "$1" || error "'$1' is not a directory!"
test -d "$2" || error "'$2' is not a directory!"
test -d "$3" || error "'$2' is not a directory!"

for depthmap in $(ls -1 $1/*png); do
  number=$(basename ${depthmap%%.png})
  number=${number##D}
  # we pipe to /dev/null because it might not have a texture.
  texture=$(ls -1 ${3}/*${number}*png 2>/dev/null)
  if test -z "${texture}"; then
    # we pipe to /dev/null because it might not have a texture: drop it, if so.
    texture=$(ls -1 ${2}/*${number}*png 2>/dev/null)
  fi
  # skip if we can't find an associated texture.
  if test -z "${texture}" ; then
    continue
  fi
  echo "Using texture: ${texture}"
  localtex=$(basename ${texture})
  echo "depthmap: ${depthmap}, ${number}, assoc tex: ${texture}, base: " \
       "${localtex}"
  cp ${texture} ${localtex} || error "copying texture failed."
  ../../meshdepth ${depthmap} ${localtex} ${number} || error "meshdepth failed!"

  ${REN} --batch -f "${SCENE}/dummy.sraf" -m "${number}.tjf" || error "ren failed."
  scp blah.png \
    shell.sci.utah.edu:/usr/sci/www/tfogal/tmp/scene/${number}-finished.png
done

ssh shell.sci.utah.edu \
  "cd /usr/sci/www/tfogal/tmp/scene/ && zip all.zip *.png"
ssh shell.sci.utah.edu chmod -R a+rX /usr/sci/www/tfogal/tmp/
