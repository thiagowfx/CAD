# Based on https://github.com/falconindy/cower/blob/master/Makefile

OUT = mergesort

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

CFLAGS    := -O2 -pedantic -Wall -Wextra -fopenmp $(CFLAGS)
LDFLAGS   := $(LDFLAGS)
LDLIBS     =

all: $(OUT)

strip: $(OUT)
	strip --strip-all $(OUT)

clean:
	$(RM) $(OUT) $(OBJ)

.PHONY: all clean
