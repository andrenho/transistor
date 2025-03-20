# this make file will generate
#  - dev version (make all)
#      - a glue .so library (transistor) which runs individual Lua files + a simulation .so library
#  - release version (make release)
#      - a single .a library that contains everything embedded


PROJECT_NAME = transistor
PROJECT_VERSION = 0.2.1

ifndef RELEASE
all: libtransistor.so sim/simulator.so test        # dev targets
else
all: transistor.a test                             # prod targets
endif

include ../contrib/pastel-base/mk/config.mk

# config

CPPFLAGS += -I. $(shell pkg-config --cflags luajit)

ifndef RELEASE  # dev version
	CPPFLAGS += -fPIC -DDEV
	LDFLAGS += $(shell pkg-config --libs luajit)

else           # release version
	CPPFLAGS += -DRELEASE
endif

# sources

ENGINE_SRC = \
	$(wildcard engine/*.tl) \
	$(wildcard engine/board/*.tl) \
	$(wildcard engine/board/geo/*.tl) \
	$(wildcard engine/tests/*.tl)

ENGINE_SRC_LUA = $(ENGINE_SRC:.tl=.lua)

API_SRC_CPP = api/transistor.o api/loader.o
API_SRC_LUA = api/api.lua

SIM_SRC = sim/simulator.o

# embed sources

ifdef RELEASE
api/loader.o: $(ENGINE_SRC_LUA:=.h) $(API_SRC_LUA:=.h)
endif

%.lua: %.tl
	tl gen -c -Iengine -Isim --gen-target 5.1 $^ -o $@

# build rules

check_tl:
	tl check -Iengine -Isim engine/*.tl engine/**.tl

libtransistor.so: $(API_SRC_CPP) $(LIB_DEPS) | check_tl
	$(CXX) -shared -fPIC $^ -o $@ $(LDFLAGS)

sim/simulator.so: $(SIM_SRC) $(LIB_DEPS)
	$(CC) -shared -fPIC $^ -o $@ $(LDFLAGS)

transistor.a: $(API_SRC_CPP) $(SIM_SRC)
	ar rcs $@ $(API_SRC_CPP) $(SIM_SRC)

ifndef RELEASE  # dev version
test: api/runtests.o | libtransistor.so sim/simulator.so
	$(CXX) -o $@ $^ $(LDFLAGS) -Wl,-rpath,. -L. -ltransistor
else            # release version
test: api/runtests.o transistor.a
	$(CXX) -o $@ $^ $(LDFLAGS) $(shell pkg-config --libs luajit)
endif

# special rules

release:
	make RELEASE=1

check: test
	./$^

clean:
	rm -f *.a *.so sim/*.so *.o **/*.o *.d **/*.d **/*.lua.h test $(ENGINE_SRC_LUA)
