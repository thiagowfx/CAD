# Based on https://github.com/falconindy/cower/blob/master/Makefile

OUT = goldbach

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

CXXFLAGS    := -std=c++11 -O2 -pedantic -Wall -Wextra -fopenmp -lm $(CXXFLAGS)
LDFLAGS   := $(LDFLAGS)
LDLIBS     =

all: $(OUT)

strip: $(OUT)
	strip --strip-all $(OUT)

clean:
	$(RM) $(OUT) $(OBJ)

.PHONY: all clean
