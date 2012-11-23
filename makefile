GLIB_CF:=`pkg-config --cflags glib-2.0`
GLIB_LD=`pkg-config --libs glib-2.0`
WARN=-Wall -Wextra -Werror
DBG=-ggdb
RLS=-O3
CFLAGS=-std=c99 $(WARN) $(DBG) $(GLIB_CF)
LIBS=-lpng $(GLIB_LD)
src := \
  bstream.c \
  crop.c \
  depth.c \
  expand.c \
  filter.c \
  lerp.c \
  obj-write.c \
  png.c \
  read.c \
  range-depth.c \
  tjf-write.c \
  scale.c
obj := $(src:.c=.o)

all: $(obj) processing meshdepth rdepth

processing: bstream.o crop.o filter.o minmax.o read.o png.o
	$(CC) $^ -o $@ $(LIBS)

meshdepth: depth.o expand.o lerp.o obj-write.o png.o scale.o tjf-write.o
	$(CC) $^ -o $@ $(LIBS)

rdepth: range-depth.o png.o
	$(CC) $^ -o $@ $(LIBS)

clean:
	rm -f $(obj) processing meshdepth rdepth
