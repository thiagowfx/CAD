# Based on https://github.com/falconindy/cower/blob/master/Makefile

OUT = matmult_serial matmult_mpi_1d

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

CC		  := mpicc
CFLAGS    := -std=c11 -O2 -pedantic -Wall -Wextra -lm $(CFLAGS)
LDFLAGS   := $(LDFLAGS)
LDLIBS     =

all: $(OUT)

strip: $(OUT)
	strip --strip-all $(OUT)

clean:
	$(RM) $(OUT) $(OBJ)

.PHONY: all clean
