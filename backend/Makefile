# this make file will generate
#  - dev version (make all)
#      - a glue .so library (transistor) which runs individual Lua files + a simulation .so library
#  - release version (make release)
#      - a single .a library that contains everything embedded


PROJECT_NAME = transistor
PROJECT_VERSION = 0.2.1

ifndef RELEASE
all: check_tl transistor.so simulator.so tests  # dev targets
else
all: transistor.a tests                         # prod targets
endif

include ../contrib/pastel-base/mk/config.mk

# rules

ifndef RELEASE
	# dev version
	CXXFLAGS += -fPIC -DDEV $(shell pkg-config --cflags luajit)
	LDFLAGS += $(shell pkg-config --libs luajit)
	TARGET := transistor.so
else
	# release version
	CXXFLAGS += -DRELEASE
	LIB_DEPS := libluajit.a
	TARGET := transistor.a
endif

# sources

ENGINE_SRC = \
	engine/sandbox.tl \
	engine/board.tl
ENGINE_SRC_LUA = $(ENGINE_SRC:.tl=.lua)

API_SRC_CPP = api/transistor.o api/loader.o
API_SRC_LUA = api/api.lua

SIM_SRC = sim/simulator.o

# embed sources

ifdef RELEASE
api/loader.o: $(ENGINE_SRC_LUA:=.h) $(API_SRC_LUA:=.h)
endif

%.lua: %.tl
	tl gen $^ -o $@

# build rules

check_tl:
	tl check -Iengine engine/*.tl engine/**.tl

transistor.so: $(API_SRC_CPP) $(LIB_DEPS)
	$(CXX) -shared -fPIC $^ -o $@ $(LDFLAGS)

simulator.so: $(SIM_SRC) $(LIB_DEPS)
	$(CC) -shared -fPIC $^ -o $@ $(LDFLAGS)

transistor.a: $(API_SRC_CPP) $(SIM_SRC)
	ar rcs $@ $(API_SRC_CPP) $(SIM_SRC)

tests: $(TARGET) runtests.o $(LIB_DEPS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# special rules

release:
	make RELEASE=1

check: tests
	./$^

clean:
	rm -f *.a *.so *.o **/*.o *.d **/*.d **/*.lua.h tests $(ENGINE_SRC_LUA)
