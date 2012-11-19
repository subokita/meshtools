WARN=-Wall -Wextra
DBG=-ggdb
RLS=-O3
CFLAGS=-std=c99 $(WARN) $(DBG)
LIBS=-lpng
src := \
  bstream.c \
  crop.c \
  depth.c \
  filter.c \
  png.c \
  read.c \
  obj-write.c
obj := $(src:.c=.o)

all: $(obj) processing meshdepth

processing: bstream.o crop.o filter.o minmax.o read.o png.o
	$(CC) $^ -o $@ $(LIBS)

meshdepth: depth.o obj-write.o png.o
	$(CC) $^ -o $@ $(LIBS)

clean:
	rm -f $(obj) processing meshdepth
