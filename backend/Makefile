# this make file will generate
#  - dev version (make all)
#      - a glue .so library (transistor) which runs individual Lua files
#  - release version (make release)
#      - a single .a library that contains everything all Lua files embedded

#
# initial rule
#

ifndef RELEASE
all: libtransistor.so test-dev    # dev targets
else
all: transistor.a test-release    # prod targets
endif

#
# configuration
#

PROJECT_NAME = transistor
PROJECT_VERSION = 0.2.1

include ../contrib/pastel-base/mk/config.mk

CPPFLAGS += -I. $(shell pkg-config --cflags luajit)
TL_CONFIG = -Iengine -Iengine/decl --gen-target 5.1

ifndef RELEASE
	CPPFLAGS += -fPIC -DDEV
	LDFLAGS += $(shell pkg-config --libs luajit)
else
	CPPFLAGS += -DRELEASE
endif

ifdef APPLE  # SDL requirements for mac
	LDFLAGS += -framework CoreFoundation -framework CoreServices -liconv -lpthread
endif

#
# sources
#

ENGINE_SRC = $(shell find engine -type f -name '*.tl')
ENGINE_SRC_LUA = $(ENGINE_SRC:.tl=.lua)

EMBEDDED_HH = interface/embedded.hh

INTERFACE_OBJ = \
	interface/transistor.o \
	interface/array.o \
	interface/render.o

#
# dev rules
#

libtransistor.so: $(INTERFACE_OBJ) $(LIB_DEPS)
	$(CXX) -shared -fPIC $^ -o $@ $(LDFLAGS)

test-dev: interface/runtests.o | libtransistor.so
	$(CXX) -o $@ $^ $(LDFLAGS) -Wl,-rpath,. -L. -ltransistor

#
# release rules
#

%.lua: %.tl
	tl gen -c ${TL_CONFIG} $^ -o $@

ifdef RELEASE
interface/transistor.o: $(ENGINE_SRC_LUA:=.h) $(EMBEDDED_HH)   # embed Lua files into the application
endif

$(EMBEDDED_HH): interface/generate_embedded.lua
	luajit $^ > $@

transistor.a: $(INTERFACE_OBJ) interface/require.o
	ar rcs $@ $^

test-release: interface/runtests.o transistor.a
	$(CXX) -o $@ $^ $(LDFLAGS) $(shell pkg-config --libs luajit)

release:
	make RELEASE=1

#
# special rules
#

check_tl:
	tl check ${TL_CONFIG} engine/*.tl engine/**/*.tl

clean:
	rm -rf *.a *.so sim/*.so *.o **/*.o *.d **/*.d test-dev test-release $(ENGINE_SRC_LUA) $(ENGINE_SRC_LUA:=.h) $(EMBEDDED_HH)

