WARN=-Wall -Wextra
CFLAGS=-std=c99 $(WARN) -ggdb
LIBS=-lpng
src := \
  bstream.c \
  cloud.c \
  crop.c \
  depth.c \
  filter.c \
  png.c \
  read.c
obj := $(src:.c=.o)

all: $(obj) processing meshdepth

processing: bstream.o cloud.o crop.o filter.o minmax.o read.o png.o
	$(CC) $^ -o $@ $(LIBS)

meshdepth: cloud.o depth.o png.o
	$(CC) $^ -o $@ $(LIBS)

clean:
	rm -f $(obj) processing meshdepth
