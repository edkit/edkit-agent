
CFLAGS = -fPIC -g -Wall -Werror -D_GNU_SOURCE
CPPFLAGS = -DFW_DEFAULT_FILENAME=\"edleak.json\"
CPPFLAGS += -DFW_DEFAULT_POLL_PERIOD=30

LDFLAGS = -l$(LDLIB_DL) -l$(LDLIB_PTHREAD)
LDFLAGS += -L../toolbox -ltoolbox
LDFLAGS += -L../thirdparty/json-c/.libs -ljson
