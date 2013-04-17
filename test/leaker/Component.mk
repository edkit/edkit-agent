
CFLAGS = -g -Wall -Werror -D_GNU_SOURCE
CPPFLAGS =

LDFLAGS = -rdynamic

SRCS =
CXX_SRCS = main.cpp

NAME = leaker
BUILD_TYPE = exe

run:
	LD_PRELOAD=$(SRCDIR)/edleak/libedleak.so edkit_fw_period=0 ./$(NAME)

include $(BUILDDIR)/srcbuild.mk

