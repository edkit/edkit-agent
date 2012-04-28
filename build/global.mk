export BUILDDIR=$(PWD)/build
export SRCDIR=$(PWD)/src

ifneq ($(ARCH),"")
ARCH:=$(ARCH)-
endif
export CC = $(ARCH)gcc
export CXX = $(ARCH)g++
export AR = $(ARCH)ar

