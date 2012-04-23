SRCS = 
CXX_SRCS = main.cpp

NAME = new1
BUILD_TYPE = exe

CFLAGS = -g -Wall
LDFLAGS = 

run: cleanruntime
	LD_PRELOAD=$(SRCDIR)/edleak/libedleak.so ./$(NAME)

clean: cleanruntime

cleanruntime:
	rm -f $(CURDIR)/*.edleak.json

include $(BUILDDIR)/srcbuild.mk

