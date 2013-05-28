C_OBJS = $(SRCS:.c=.o)
CXX_OBJS = $(CXX_SRCS:.cpp=.o)
OBJS = $(C_OBJS) $(CXX_OBJS)

BUILD_TYPE_EXE=exe
BUILD_TYPE_STATIC_LIB=staticlib
BUILD_TYPE_DYN_LIB=dynlib
# exe
EXE_BUILD = $(findstring $(BUILD_TYPE_EXE),$(BUILD_TYPE))
ifeq ($(EXE_BUILD),$(BUILD_TYPE_EXE))
TARGET_NAME = $(NAME)
OUT=$(BUILD_TYPE_EXE)
endif

# static lib
STATIC_BUILD = $(findstring $(BUILD_TYPE_STATIC_LIB),$(BUILD_TYPE))
ifeq ($(STATIC_BUILD),$(BUILD_TYPE_STATIC_LIB))
TARGET_NAME = lib$(NAME).a
OUT=$(BUILD_TYPE_STATIC_LIB)
endif

# dynamic lib
DYN_BUILD = $(findstring $(BUILD_TYPE_DYN_LIB),$(BUILD_TYPE))
ifeq ($(DYN_BUILD),$(BUILD_TYPE_DYN_LIB))
TARGET_NAME = lib$(NAME).so
OUT=$(BUILD_TYPE_DYN_LIB)
endif

# sub-components
SUB_COMPONENTS_RULES:=$(addsuffix .subcomponent,$(SUB_COMPONENTS))
SUB_COMPONENTS_RULES_CLEAN:=$(addsuffix .subcomponent_clean,$(SUB_COMPONENTS))

# common compiler options
CPPFLAGS+=-I$(CURDIR)/src -I$(CURDIR)/include

# common targets
all: $(SUB_COMPONENTS_RULES) $(OUT)

exe: $(OBJS)
	$(CXX) -o $(NAME) $(OBJS) $(LDFLAGS)

staticlib: $(OBJS)
	$(AR) -rcs lib$(NAME).a $(OBJS)

dynlib: $(OBJS)
	$(CXX) -shared -Wl,-soname,$(TARGET_NAME) -o $(TARGET_NAME) $(OBJS) $(LDFLAGS)

clean: $(SUB_COMPONENTS_RULES_CLEAN)
	rm -f $(C_OBJS)
	rm -f $(CXX_OBJS)
	rm -f $(TARGET_NAME)

%.o : %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

%.o : %.cpp
	$(CXX) $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) -o $@ -c $<

%.subcomponent:
	make -f Component.mk -C $(CURDIR)/$(basename $@)

%.subcomponent_clean:
	make -f Component.mk -C $(CURDIR)/$(basename $@) clean

