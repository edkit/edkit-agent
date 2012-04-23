include ./configure.in
include ./build/global.mk

all: jsonc cppunit toolbox edleak

test_all: toolbox_test edleak_test

test_run: toolbox_test_run edleak_test_run

test_clean: toolbox_test_clean edleak_test_clean

clean: toolbox_clean edleak_clean

toolbox:
	$(MAKE) -C src/toolbox -f Component.mk all

toolbox_test:
	$(MAKE) -C test/toolbox -f Component.mk all

toolbox_test_run:
	$(MAKE) -C test/toolbox -f Component.mk run

toolbox_test_clean:
	$(MAKE) -C test/toolbox -f Component.mk clean

toolbox_clean:
	$(MAKE) -C src/toolbox -f Component.mk clean

edleak:
	$(MAKE) -C src/edleak -f Component.mk all

edleak_test:
	$(MAKE) -C test/edleak -f Component.mk all

edleak_test_run:
	$(MAKE) -C test/edleak -f Component.mk run

edleak_test_clean:
	$(MAKE) -C test/edleak -f Component.mk clean

edleak_clean:
	$(MAKE) -C src/edleak -f Component.mk clean

jsonc:
	$(MAKE) -C src/thirdparty/json-c

cppunit:
	$(MAKE) -C src/thirdparty/cppunit

