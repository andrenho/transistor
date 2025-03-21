# this make file will generate
#  - dev version (make all)
#      - a glue .so library (transistor) which runs individual Lua files
#  - release version (make release)
#      - a single .a library that contains everything all Lua files embedded


PROJECT_NAME = transistor
PROJECT_VERSION = 0.2.1

ifndef RELEASE
all: libtransistor.so test-dev    # dev targets
else
all: transistor.a test-release    # prod targets
endif

include ../contrib/pastel-base/mk/config.mk

# config

CPPFLAGS += -I. $(shell pkg-config --cflags luajit)
TL_CONFIG = -Iengine -Iengine/contrib --gen-target 5.1

ifndef RELEASE  # dev version
	CPPFLAGS += -fPIC -DDEV
	LDFLAGS += $(shell pkg-config --libs luajit)

else           # release version
	CPPFLAGS += -DRELEASE
endif

# sources

ENGINE_SRC = $(shell find engine -type f -name '*.tl')
ENGINE_SRC_LUA = $(ENGINE_SRC:.tl=.lua)

API_SRC_CPP = api/transistor.o api/loader.o

# embed sources

ifdef RELEASE
api/loader.o: $(ENGINE_SRC_LUA:=.h)    # embed Lua files into the application
endif

%.lua: %.tl
	tl gen -c ${TL_CONFIG} $^ -o $@

# dev rules

libtransistor.so: $(API_SRC_CPP) $(LIB_DEPS) | check_tl
	$(CXX) -shared -fPIC $^ -o $@ $(LDFLAGS)

test-dev: api/runtests.o | libtransistor.so
	$(CXX) -o $@ $^ $(LDFLAGS) -Wl,-rpath,. -L. -ltransistor

# release rules

transistor.a: $(API_SRC_CPP) $(SIM_SRC)
	ar rcs $@ $(API_SRC_CPP) $(SIM_SRC)

test-release: api/runtests.o transistor.a
	$(CXX) -o $@ $^ $(LDFLAGS) $(shell pkg-config --libs luajit)

release:
	make RELEASE=1

# special rules

check_tl:
	tl check ${TL_CONFIG} engine/*.tl engine/**/*.tl

clean:
	rm -rf *.a *.so sim/*.so *.o **/*.o *.d **/*.d test-dev test-release $(ENGINE_SRC_LUA) $(ENGINE_SRC_LUA:=.h)

