SRCS = main.c
CXX_SRCS = 

NAME = malloc1
BUILD_TYPE = exe

CFLAGS = -g -Wall
LDFLAGS = -export-dynamic

include ../../../build/arch.make
include ../../../build/makefile.gen

run: cleanruntime
	LD_PRELOAD=../../../src/edleak/libedleak.so edkit_fw_period=6 ./$(NAME)

cleanall: clean cleanruntime

cleanruntime:
	rm -f edleak.json
