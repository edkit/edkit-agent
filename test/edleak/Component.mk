
default: all

all:
	$(MAKE) -C $(CURDIR)/calloc1 -f Component.mk all
	$(MAKE) -C $(CURDIR)/malloc1 -f Component.mk all
	$(MAKE) -C $(CURDIR)/realloc1 -f Component.mk all
	$(MAKE) -C $(CURDIR)/realloc2_null -f Component.mk all
	$(MAKE) -C $(CURDIR)/new1_nothrow -f Component.mk all
	$(MAKE) -C $(CURDIR)/new2_throw -f Component.mk all

run:
	$(MAKE) -C $(CURDIR)/calloc1 -f Component.mk run
	$(MAKE) -C $(CURDIR)/malloc1 -f Component.mk run
	$(MAKE) -C $(CURDIR)/realloc1 -f Component.mk run
	$(MAKE) -C $(CURDIR)/realloc2_null -f Component.mk run
	$(MAKE) -C $(CURDIR)/new1_nothrow -f Component.mk run
	$(MAKE) -C $(CURDIR)/new2_throw -f Component.mk run

clean:
	$(MAKE) -C $(CURDIR)/calloc1 -f Component.mk clean
	$(MAKE) -C $(CURDIR)/malloc1 -f Component.mk clean
	$(MAKE) -C $(CURDIR)/realloc1 -f Component.mk clean
	$(MAKE) -C $(CURDIR)/realloc2_null -f Component.mk clean
	$(MAKE) -C $(CURDIR)/new1_nothrow -f Component.mk clean
	$(MAKE) -C $(CURDIR)/new2_throw -f Component.mk clean

