all: transistor.so simulator.so

include ../contrib/pastel-base/mk/config.mk

# rules

ifndef RELEASE
	CXXFLAGS += -fPIC
endif

# sources

ENGINE_SRC = engine/sandbox.tl

API_SRC_CPP = api/transistor.o
API_SRC_LUA = api/api.lua

SIM_SRC = sim/simulator.o

# rules

ifdef RELEASE
release:
	make RELEASE=1
else
release: transistor.a
endif

transistor.so: $(API_SRC_CPP)
	gcc -shared -fPIC $^ -o $@ $(LDFLAGS)

simulator.so: $(SIM_SRC)
	gcc -shared -fPIC $^ -o $@ $(LDFLAGS)

transistor.a: $(API_SRC_CPP) $(SIM_SRC)
	ar rcs $@ $^

clean:
	rm -f $(API_SRC_CPP) $(SIM_SRC) transistor.so simulator.so transistor.o