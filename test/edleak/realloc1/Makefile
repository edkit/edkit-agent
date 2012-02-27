SRCS = main.c
CXX_SRCS = 

NAME = realloc1
BUILD_TYPE = exe

CFLAGS = -g -Wall
LDFLAGS = 

include ../../../build/arch.make
include ../../../build/makefile.gen

run: cleanruntime
	LD_PRELOAD=../../../src/edleak/libedleak.so ./$(NAME)

cleanall: clean cleanruntime

cleanruntime:
	rm -f *.edleak.json
