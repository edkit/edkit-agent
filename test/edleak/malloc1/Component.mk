SRCS = main.c
CXX_SRCS = 

NAME = malloc1
BUILD_TYPE = exe

CFLAGS = -g -Wall
LDFLAGS = -export-dynamic

run: cleanruntime
	LD_PRELOAD=$(SRCDIR)/edleak/libedleak.so edkit_fw_period=6 ./$(NAME)

clean: cleanruntime

cleanruntime:
	rm -f $(CURDIR)/*.edleak.json

include $(BUILDDIR)/srcbuild.mk

