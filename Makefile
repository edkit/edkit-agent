include ./configure.in
include ./build/global.mk

all: jsonc toolbox edleak

test_all: cppunit cpputest toolbox_test edleak_test leaker

test_run: toolbox_test_run edleak_test_run

test_clean: toolbox_test_clean edleak_test_clean cpputest_clean leaker_clean

clean: test_clean jsonc_clean toolbox_clean edleak_clean

toolbox:
	$(MAKE) -C src/toolbox -f Component.mk all

toolbox_clean:
	$(MAKE) -C src/toolbox -f Component.mk clean

toolbox_test:
	$(MAKE) -C test/toolbox -f Component.mk all

toolbox_test_run:
	$(MAKE) -C test/toolbox -f Component.mk run

toolbox_test_clean:
	$(MAKE) -C test/toolbox -f Component.mk clean

edleak:
	$(MAKE) -C src/edleak -f Component.mk all

edleak_clean:
	$(MAKE) -C src/edleak -f Component.mk clean


edleak_test:
	$(MAKE) -C test/edleak -f Component.mk all

edleak_test_run:
	$(MAKE) -C test/edleak -f Component.mk run

edleak_test_clean:
	$(MAKE) -C test/edleak -f Component.mk clean

leaker:
	$(MAKE) -C test/leaker -f Component.mk all

leaker_run:
	$(MAKE) -C test/leaker -f Component.mk run

leaker_clean:
	$(MAKE) -C test/leaker -f Component.mk clean

jsonc:
	$(MAKE) -C src/thirdparty/json-c

jsonc_clean:
	$(MAKE) -C src/thirdparty/json-c clean

cppunit:
	$(MAKE) -C src/thirdparty/cppunit

cpputest:
	$(MAKE) -C src/thirdparty/cpputest CPPUTEST_USE_STD_CPP_LIB=Y all_no_tests extensions_no_tests

cpputest_clean:
	$(MAKE) -C src/thirdparty/cpputest clean cleanExtensions
