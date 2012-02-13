
CFLAGS = -g -Wall -Werror -D_GNU_SOURCE

LDFLAGS = -L$(LD_LIBRARY_PATH) -l$(LDLIB_DL)
LDFLAGS += -L../../src/toolbox -ltoolbox
LDFLAGS += -L../../src/thirdparty/json-c/.libs -ljson
LDFLAGS += -lcppunit
